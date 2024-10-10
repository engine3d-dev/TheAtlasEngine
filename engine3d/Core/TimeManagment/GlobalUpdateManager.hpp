#pragma once
#include <Window.hpp>
#include <print>

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
            static float GlobalDeltaTime;

            /// <summary>
            /// Adding the two bools may allow us to seperate the game
            /// from the editor. Allowing us to stop one without stopping both.
            /// We still need to think about how reverting back will work.
            /// </summary>
            bool isRunning;
            bool editorRunning;

        public:
            // May change to inline
            static const float& globalDeltaTime;


            ///<remark>
            /// Do we make this a singleton and make this private?
            /// <\remark>
            GlobalUpdateManager();

            // Manages the sync of each updateManager if required
            void globalOnTickUpdate();

            // Maintains a const fps if possible
            void waitForNextFrame();

    };
};