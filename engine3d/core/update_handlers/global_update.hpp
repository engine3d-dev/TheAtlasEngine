#pragma once
#include <core/update_handlers/thread_manager.hpp>
#include <core/update_handlers/timer.hpp>
#include <deque>
#include <functional>
#include <span>
#include <array>

namespace engine3d{
    class GlobalUpdate{

    public:
        /*
        * NEEDS FIX: Change to a more secure way to always have one
        * GlobalUpdate per application instance
        */ 
        GlobalUpdate() = delete;
        ~GlobalUpdate() = delete;
        GlobalUpdate(const GlobalUpdate&) = delete;
        GlobalUpdate& operator=(const GlobalUpdate&) = delete;
        
        //! @note Initializes the global update manager and its cleanup function
        static void Initialize();
        static void GlobalCleanup();

        /*
        * NEEDS FIX: Should not create raw pointers in constructors
        * and deleting them in destructor. Unsafe!
        */
        //! @note update specialization
        template<typename, typename = std::void_t<>>
        struct m_HasUpdate : std::false_type{};

        template<typename UCompClass>
        struct m_HasUpdate<UCompClass, std::void_t<decltype(
            std::declval<UCompClass>().OnApplicationUpdate())>> : std::true_type {};

        /*
            SubscribeApplicationUpdate
            - We wont need Subscription methods for application-specifically
            - This is because there should only ever be one application running at once
            - Sounds redundant to having a single subscription for application update
            - Application Responsibilities are the folloiwng
                - Serializing/Deserializing project/game properties
                - Setting up game-state, and systematic utilities
                - ensuring the lifetimes of specific objects at pre-init/initialization such as world state, and scenes of the game (environment maps, etc)
                - Multiple configurations that would be looked at for specifically the runtime engine3d-runtime

        */
        template<typename UObject, typename UFunction>
        static void SubscribeApplicationUpdate(UObject* p_Instance, const UFunction&& p_Update){
            // ConsoleLogError("When do you get called!!!");
            if constexpr (m_HasUpdate<UObject>::value){
                ConsoleLogError("When are you getting called (2)!!");
                if(&UObject::OnApplicationUpdate == p_Update){
                    s_ApplicationUpdates.push_back([p_Instance, p_Update](){
                        (p_Instance->*p_Update)();
                    });
                }
            }
            else{
                ConsoleLogFatal("Cannot subscript to that function!");
            }

            //! @note We should not halt the application by throwing an error this way
            //! @note What we should do is if the user subscribes an invalid callable function, dont do anything but provide error messages of the error
            // throw std::runtime_error
            //     ("Faulty subscribed function!\n \tUse update, lateUpdate, onTickUpdate!");
        }

        // Manages the sync of each updateManager if required
        static void GlobalOnTickUpdate();

        // Maintains a const fps if possible
        static void WaitForNextFrame();

        static void IncrementCounter();

    private:
        /* Using std::deque because faster insertion/deletion then std::vector */
        /* we do not need to randomly write to the queue */
        static std::deque<std::function<void()>> s_ApplicationUpdates;

        // Scope<ThreadManager> m_threadManager = nullptr;
        // static ThreadManager s_ThreadManager;
        // std::vector<std::function<void()>> m_ApplicationUpdateSubscribers;
        // //!  @note represents the deltaTime for all applications
        // float m_GlobalDeltaTime;
        // std::chrono::time_point<std::chrono::high_resolution_clock> m_UpdateTime;
        // Timer* m_GlobalTimer;
        // Timer* m_FPSMaintain;

    };
};