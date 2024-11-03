#pragma once
#include <Core/TimeManagement/Timer.hpp>
#include <Core/Event/InputPoll.hpp>
#include <functional>
#include <chrono>
namespace engine3d
{

    static std::vector<std::function<void(float)>> m_SyncLateUpdateSubscribers;
    static std::vector<std::function<void(float)>> m_SyncUpdateSubscribers;
    static std::vector<std::function<void(float)>> m_SyncOnTickUpdateSubscribers;
    class SyncUpdateManager
    {
        private:
            SyncUpdateManager();
            std::chrono::time_point<std::chrono::high_resolution_clock> m_LocalUpdateTime;
            
            int m_MaxVariance;
            int m_MinFrames;

            static SyncUpdateManager* m_Manager;

            Timer* m_LocalTimer;
            InputPoll* m_KeyEvent;
            float m_LocalDeltaTime;
            int m_LocalUpdateCounter;
            int m_RandomFrame;

            //! @note inaccurate fps because it relies on window to call.
            //! @note Jframe may solve issue but yet to be tested.
            int m_LocalFPS;

            // Called EveryFrame
            void OnPhysicsUpdate(float p_DeltaTime);

            // Varied by random frames
            void OnUpdate(float p_DeltaTime);
            void OnLateUpdate(float p_DeltaTime);

            //! @note update specialization
            template<typename, typename = std::void_t<>>
            struct m_HasUpdate : std::false_type{};

            template<typename UCompClass>
            struct m_HasUpdate<UCompClass, std::void_t<decltype(
                declval<UCompClass>().Update(0.0f))>> : std::true_type {};

            //! @note lateUpdate specialization
            template<typename, typename = std::void_t<>>
            struct m_HasLateUpdate : std::false_type{};

            template<typename UCompClass>
            struct m_HasLateUpdate<UCompClass, std::void_t<decltype(
                declval<UCompClass>().FrameUpdate(0.0f))>> : std::true_type {};

            //! @note onTickUpdate specialization
            template<typename, typename = std::void_t<>>
            struct m_HasPhysicsUpdate : std::false_type{};

            template<typename UCompClass>
            struct m_HasPhysicsUpdate<UCompClass, std::void_t<decltype(
                declval<UCompClass>().PhysicsUpdate(0.0f))>> : std::true_type {};

        public:
            ~SyncUpdateManager();

            //Called by threadManager
            void RunUpdate(float deltaTime);

            //Component Subscription
            template<typename UComponent, typename UFunction>
            static void Subscribe(UComponent* p_Instance, const UFunction&& p_Update)
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
                    if(&UComponent::OnTickUpdate == p_Update)
                    {
                        m_SyncOnTickUpdateSubscribers.push_back([p_Instance, p_Update]() 
                        {
                            (p_Instance->*p_Update)();
                        });
                        return;
                    }
                }
                throw runtime_error("Faulty subscribed function!\n \tUse update, lateUpdate, onTickUpdate!");
            }
    };
};