#pragma once
// #include <Window.hpp>
#include <Core/ApplicationManager/ThreadMngr.hpp>
#include <Core/TimeManagement/UpdateManagers/SyncUpdateManager.hpp>
#include <Physics/JoltHandler.hpp>
#include <Core/EngineLogger.hpp>

namespace engine3d
{
    /**
    * @name GlobalUpdateManager
    * @note Used to keep track of updates and frame data.
    * @note Handles the other updateManagers.
    */

    class GlobalUpdateManager
    {
        private:
            //!  @note represents the deltaTime for all applications
            float m_GlobalDeltaTime;
            std::chrono::time_point<std::chrono::high_resolution_clock> m_UpdateTime;
            Timer* m_GlobalTimer;
            Timer* m_FPSMaintain;

            float m_MaxFPS;
            int m_FPSCounter;

        public:
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

            template<typename UComponent, typename UFunction>
            void SubscribeApplicationUpdate(UComponent* p_Instance, const UFunction&& p_Update)
            {
                ConsoleLogError("When do you get called!!!");
                ConsoleLogError("m_HasUpdate = {}", m_HasUpdate<UComponent>::value);
                if constexpr (m_HasUpdate<UComponent>::value){
                    ConsoleLogError("When are you getting called (2)!!");
                    if(&UComponent::OnApplicationUpdate == p_Update)
                    {
                        m_ApplicationUpdateSubscribers.push_back([p_Instance, p_Update]() 
                        {
                            (p_Instance->*p_Update)();
                        });
                        return;
                    }
                }
                throw std::runtime_error
                    ("Faulty subscribed function!\n \tUse update, lateUpdate, onTickUpdate!");
            }

            static GlobalUpdateManager* GetInstance() 
            {
                static GlobalUpdateManager instance;
                return &instance;
            }

            /*
            * NEEDS FIX: Change to a more secure way to always have one
            * GlobalUpdateManager per application instance
            */ 
            GlobalUpdateManager();
            ~GlobalUpdateManager();

            // Manages the sync of each updateManager if required
            void GlobalOnTickUpdate();

            void OnSetUp();

            // Maintains a const fps if possible
            void WaitForNextFrame();

            ThreadMngr * m_threadManager = NULL;

            std::vector<std::function<void()>> m_ApplicationUpdateSubscribers;

            GlobalUpdateManager(const GlobalUpdateManager&) = delete;
            GlobalUpdateManager& operator=(const GlobalUpdateManager&) = delete;
    };
};
