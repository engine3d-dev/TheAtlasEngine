#pragma once
// #include <Window.hpp>
#include <Core/TimeManagement/UpdateManagers/SyncUpdateManager.hpp>
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

            //! @note for testing purposes can delete later!
            SyncUpdateManager* t_syncUpdate;

            float m_MaxFPS;
            int counter;
            

            /*
            * NEEDS FIX: Should not create raw pointers in constructors
            * and deleting them in destructor. Unsafe!
            */ 
 
            InputPoll* m_keyEvent;

        public:
            static GlobalUpdateManager* m_Instance;

            /*
            * NEEDS FIX: Change to a more secure way to always have one
            * GlobalUpdateManager per application instance
            */ 
            GlobalUpdateManager();
            ~GlobalUpdateManager();

            // Manages the sync of each updateManager if required
            void globalOnTickUpdate();

            void OnSetUp();

            // Maintains a const fps if possible
            void waitForNextFrame();

    };
};