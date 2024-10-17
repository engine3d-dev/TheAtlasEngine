#pragma once
#include "TimeManagement/UpdateParent/SyncUpdate.hpp"
#include <Core/Event/InputPoll.hpp>
#include <chrono>
#include <Core/TimeManagement/Timer.hpp>
namespace engine3d
{
    class SyncUpdateManager
    {
        public:
            SyncUpdateManager();
            ~SyncUpdateManager();

            //Called by threadManager
            void runUpdate(float deltaTime, SyncUpdate* start, SyncUpdate* end);
        protected:
            // Called EveryFrame
            virtual void onTickUpdate(float deltaTime) {};

            //! Rendering @note might be varied?
            virtual void render(float deltaTime, int randomFrame) {};

            // Varied by random frames
            virtual void update(float deltaTime) {};
            virtual void lateUpdate(float deltaTime) {};

        private:
            std::chrono::time_point<std::chrono::high_resolution_clock> m_localUpdateTime;
            
            int m_maxVariance;
            int m_minFrames;

            Timer* m_LocalTimer;
            InputPoll* m_keyEvent;
            float m_localDeltaTime;
            int m_localUpdateCounter;
            int m_randomFrame;

            //! @note inaccuate fps because it relies on window to call.
            //! @note Jframe may solve issue but yet to be tested.
            int m_localFPS;

            float test;
    };
};