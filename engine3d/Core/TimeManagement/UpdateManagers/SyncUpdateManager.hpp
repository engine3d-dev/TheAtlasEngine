#pragma once
#include <Core/TimeManagement/Timer.hpp>
// #include <Core/Event/InputPoll.hpp>
#include <functional>
#include <chrono>
#include <stdexcept>
namespace engine3d
{

    class SyncUpdateManager
    {
        private:
            std::chrono::time_point<std::chrono::high_resolution_clock> m_LocalUpdateTime;
            
            int m_MaxVariance;
            int m_MinFrames;

            Timer* m_LocalTimer;
            // InputPoll* m_KeyEvent;
            int m_LocalUpdateCounter;
            int m_RandomFrame;

            //! @note inaccurate fps because it relies on window to call.
            //! @note Jframe may solve issue but yet to be tested.
            int m_LocalFPS;

            // Called EveryFrame
            void OnPhysicsUpdate();

            // Varied by random frames
            void OnUpdate();
            void OnLateUpdate();
            void OnSceneRender();

            //! @note update specialization
            template<typename, typename = std::void_t<>>
            struct m_IsRenderer : std::false_type{};

            template<typename UCompClass>
            struct m_IsRenderer<UCompClass, std::void_t<decltype(
                std::declval<UCompClass>().RenderScenes())>> : std::true_type {};

            //! @note update specialization
            template<typename, typename = std::void_t<>>
            struct m_HasUpdate : std::false_type{};

            template<typename UCompClass>
            struct m_HasUpdate<UCompClass, std::void_t<decltype(
                std::declval<UCompClass>().Update())>> : std::true_type {};

            //! @note lateUpdate specialization
            template<typename, typename = std::void_t<>>
            struct m_HasLateUpdate : std::false_type{};

            template<typename UCompClass>
            struct m_HasLateUpdate<UCompClass, std::void_t<decltype(
                std::declval<UCompClass>().LateUpdate())>> : std::true_type {};

            //! @note onTickUpdate specialization
            template<typename, typename = std::void_t<>>
            struct m_HasPhysicsUpdate : std::false_type{};

            template<typename UCompClass>
            struct m_HasPhysicsUpdate<UCompClass, std::void_t<decltype(
                std::declval<UCompClass>().PhysicsUpdate())>> : std::true_type {};

        public:
            std::vector<std::function<void()>> m_SyncLateUpdateSubscribers;
            std::vector<std::function<void()>> m_SyncUpdateSubscribers;
            std::vector<std::function<void()>> m_SyncOnTickUpdateSubscribers;
            std::vector<std::function<void()>> m_SyncRenderSubscribers;

            SyncUpdateManager();
            ~SyncUpdateManager();
            float m_SyncLocalDeltaTime;
            float m_SyncGlobalDeltaTime;
            //Called by threadManager
            void RunUpdate(float deltaTime);

            //Component Subscription
            template<typename UComponent, typename UFunction>
            void Subscribe(UComponent* p_Instance, const UFunction&& p_Update)
            {

                if constexpr (m_HasUpdate<UComponent>::value)
                {
                    if(&UComponent::Update == p_Update)
                    {
                        m_SyncUpdateSubscribers.push_back([p_Instance, p_Update]() 
                        {
                            (p_Instance->*p_Update)();
                        });
                        return;
                    }
                }
                if constexpr (m_HasLateUpdate<UComponent>::value)
                {
                    if(&UComponent::LateUpdate == p_Update)
                    {
                        m_SyncLateUpdateSubscribers.push_back([p_Instance, p_Update]() 
                        {
                            (p_Instance->*p_Update)();
                        });
                        return;
                    }
                }
                if constexpr (m_HasPhysicsUpdate<UComponent>::value)
                {
                    if(&UComponent::PhysicsUpdate == p_Update)
                    {
                        m_SyncOnTickUpdateSubscribers.push_back([p_Instance, p_Update]() 
                        {
                            (p_Instance->*p_Update)();
                        });
                        return;
                    }
                }

                // For scenes not components
                if constexpr (m_IsRenderer<UComponent>::value)
                {
                    if(&UComponent::RenderScenes == p_Update)
                    {
                        m_SyncRenderSubscribers.push_back([p_Instance, p_Update]() 
                        {
                            (p_Instance->*p_Update)();
                        });
                        return;
                    }
                }
                throw std::runtime_error
                    ("Faulty subscribed function!\n \tUse update, lateUpdate, onTickUpdate!");
            }

            static SyncUpdateManager* GetInstance() 
            {
                static SyncUpdateManager instance;
                return &instance;
            }
            SyncUpdateManager(const SyncUpdateManager&) = delete;
            SyncUpdateManager& operator=(const SyncUpdateManager&) = delete;
    };
};
