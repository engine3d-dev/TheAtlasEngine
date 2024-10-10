#pragma once
#include <Window.hpp>
#include <print>
#include "UpdateManagers/ParallelFrameUpdateManager.hpp"
#include "UpdateManagers/SyncUpdateManager.hpp"

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
            float GlobalDeltaTime;

            /*
            * NEEDS FIX: Should not create raw pointers in constructors
            * and deleting them in destructor. Unsafe!
            */ 
            ParallelFrameUpdateManager* frameUpdateManager;
            SyncUpdateManager* syncUpdateManger;

        public:

            /*
            * NEEDS FIX: Change to a more secure way to always have one
            * GlobalUpdateManager per application instance
            */ 
            GlobalUpdateManager();
            ~GlobalUpdateManager();

            // Manages the sync of each updateManager if required
            void globalOnTickUpdate();

            // Maintains a const fps if possible
            void waitForNextFrame();

    };
};