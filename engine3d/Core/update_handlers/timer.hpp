#pragma once
#include <chrono>

namespace engine3d{
    class Timer{

        public:
        Timer();

        void Reset();

        float Elapsed();

        float ElapsedSec();

        std::chrono::time_point<std::chrono::high_resolution_clock> GetCurrentTime();

        private:
            std::chrono::time_point<std::chrono::high_resolution_clock> m_StopWatch;

    };
};