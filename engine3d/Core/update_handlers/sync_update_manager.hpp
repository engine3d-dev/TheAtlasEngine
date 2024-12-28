#pragma once
#include <core/engine_logger.hpp>
#include <core/update_handlers/timer.hpp>
#include <functional>
#include <chrono>
#include <stdexcept>

namespace engine3d{
    class SyncUpdateManager{
    public:
        SyncUpdateManager() = delete;
        SyncUpdateManager(const SyncUpdateManager&) = delete;
        SyncUpdateManager& operator=(const SyncUpdateManager&) = delete;
        ~SyncUpdateManager();

        //! @note Initializes sync update manager to make sure our manager is able to dispatch our update functions.
        static void InitializeSyncUpdate();

        // Called by threadManager
        static void RunUpdate(float deltaTime);

        //! @note We are subscribing our update functions to be dispatched at different times.
        /**
         * @param UObject is the object that contains the defined function benig subscribed to
         * @param UFunction is the defined update function that corresponded to the function instance
        */
        template<typename UObject, typename UFunction>
        static void Subscribe(UObject* p_Instance, const UFunction&& p_Update){
            if constexpr (HasUpdate<UObject>::value){
                if(&UObject::OnUpdate == p_Update){
                    s_SyncUpdateSubscribers.push_back([p_Instance, p_Update]() {
                        (p_Instance->*p_Update)();
                    });
                }
            }
            else if constexpr (HasLateUpdate<UObject>::value){
                if(&UObject::LateUpdate == p_Update){
                    s_SyncLateUpdateSubscribers.push_back([p_Instance, p_Update]() {
                        (p_Instance->*p_Update)();
                    });
                    return;
                }
            }
            else if constexpr (HasPhysicsUpdate<UObject>::value){
                if(&UObject::PhysicsUpdate == p_Update)
                {
                    s_SyncOnTickUpdateSubscribers.push_back([p_Instance, p_Update]() 
                    {
                        (p_Instance->*p_Update)();
                    });
                    return;
                }
            }
            else{
                ConsoleLogFatal("Faulted Subscribing Function!");
            }

            // For scenes not components
            //! @note [My Review] - This probably wont be handled by the subscription.
            // if constexpr (IsRenderer<UComponent>::value)
            // {
            //     if(&UComponent::RenderScenes == p_Update)
            //     {
            //         m_SyncRenderSubscribers.push_back([p_Instance, p_Update]() 
            //         {
            //             (p_Instance->*p_Update)();
            //         });
            //         return;
            //     }
            // }
        }


    private:

        // Called EveryFrame
        static void OnPhysicsUpdate();

        // // Varied by random frames
        static void OnUpdate();
        static void OnLateUpdate();
        static void OnSceneRender();

        // This IsRender template specialization is not needed.
        //! @note update specialization
        // template<typename, typename = std::void_t<>>
        // struct IsRenderer : std::false_type{};

        // // RenderScenes (?)
        // // This, I will comment out.
        // // // I am considering replacing this with the scene renderer. When I start looking into the scene graph stuff.
        // template<typename UCompClass>
        // struct IsRenderer<UCompClass, std::void_t<decltype(
        //     std::declval<UCompClass>().RenderScenes())>> : std::true_type {};

        //! @note update specialization
        template<typename, typename = std::void_t<>>
        struct HasUpdate : std::false_type{};

        template<typename UCompClass>
        struct HasUpdate<UCompClass, std::void_t<decltype(
            std::declval<UCompClass>().OnUpdate())>> : std::true_type {};

        //! @note lateUpdate specialization
        template<typename, typename = std::void_t<>>
        struct HasLateUpdate : std::false_type{};

        template<typename UCompClass>
        struct HasLateUpdate<UCompClass, std::void_t<decltype(
            std::declval<UCompClass>().LateUpdate())>> : std::true_type {};

        //! @note onTickUpdate specialization
        template<typename, typename = std::void_t<>>
        struct HasPhysicsUpdate : std::false_type{};

        template<typename UCompClass>
        struct HasPhysicsUpdate<UCompClass, std::void_t<decltype(
            std::declval<UCompClass>().PhysicsUpdate())>> : std::true_type {};
    private:
        //! @note is there a better way of doing this?
        static std::vector<std::function<void()>> s_SyncLateUpdateSubscribers;
        static std::vector<std::function<void()>> s_SyncUpdateSubscribers;
        static std::vector<std::function<void()>> s_SyncOnTickUpdateSubscribers;
        static std::vector<std::function<void()>> s_SyncRenderSubscribers;
    };
};