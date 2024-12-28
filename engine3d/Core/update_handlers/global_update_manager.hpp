#pragma once
// #include "core.hpp"
#include <core/update_handlers/thread_manager.hpp>
#include <core/update_handlers/timer.hpp>
#include <functional>

namespace engine3d{
    class GlobalUpdateManager{

    public:
        /*
        * NEEDS FIX: Change to a more secure way to always have one
        * GlobalUpdateManager per application instance
        */ 
        GlobalUpdateManager() = delete;
        ~GlobalUpdateManager() = delete;
        GlobalUpdateManager(const GlobalUpdateManager&) = delete;
        GlobalUpdateManager& operator=(const GlobalUpdateManager&) = delete;
        
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

        template<typename UObject, typename UFunction>
        static void SubscribeApplicationUpdate(UObject* p_Instance, const UFunction&& p_Update){
            // ConsoleLogError("When do you get called!!!");
            // ConsoleLogError("m_HasUpdate = {}", m_HasUpdate<UObject>::value);
            if constexpr (m_HasUpdate<UObject>::value){
                ConsoleLogError("When are you getting called (2)!!");
                if(&UObject::OnApplicationUpdate == p_Update){
                    s_ApplicationUpdateSubscribers.push_back([p_Instance, p_Update]() 
                    {
                        (p_Instance->*p_Update)();
                    });
                    
                    return;
                }
            }
            else{
                ConsoleLogFatal("Cannot subscript to that function!");
            }
            // throw std::runtime_error
            //     ("Faulty subscribed function!\n \tUse update, lateUpdate, onTickUpdate!");
        }

        // Manages the sync of each updateManager if required
        static void GlobalOnTickUpdate();

        // static void OnSetUp();

        // Maintains a const fps if possible
        static void WaitForNextFrame();

        static void IncrementCounter();

    private:
        static std::vector<std::function<void()>> s_ApplicationUpdateSubscribers;
        // Scope<ThreadManager> m_threadManager = nullptr;
        // static ThreadManager s_ThreadManager;
        // std::vector<std::function<void()>> m_ApplicationUpdateSubscribers;
        // //!  @note represents the deltaTime for all applications
        // float m_GlobalDeltaTime;
        // std::chrono::time_point<std::chrono::high_resolution_clock> m_UpdateTime;
        // Timer* m_GlobalTimer;
        // Timer* m_FPSMaintain;

        // float m_MaxFPS;
        // int m_FPSCounter;
    };
};