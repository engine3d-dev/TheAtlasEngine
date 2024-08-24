#pragma once
#include <cstdint>

namespace engine3d{
    /**
     * 
     * @name FrameTimer
     * @note Timer that is used to define our delta-time
     * @note Definition that my be used across the entire engine to help with synchronizing the framerates. 
     * @note Our framerate is known globally throughout Engine3D.
    */
    class FrameTimer{
    public:
        //! @note Used for updating per frame time.
        static void UpdateFrameTimer();

        //! @note Getting our frames either per sec, millsecond, nanoseconds
        static float GetSeconds();
        static float GetMilliseconds();
    };
};