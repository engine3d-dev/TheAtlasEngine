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

            /// <remarks>
            /// Do we want to make this just a getter? Or is this good syntax?
            /// </remarks>

            static const float& globalDeltaTime;

            void setupConfig();

            // Manages the sync of each updateManager if required
            void globalOnTickUpdate();
    };
};