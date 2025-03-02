#pragma once
#include <chrono>

namespace atlas {
    class timer {

    public:
        timer();

        void reset();

        float elapsed();

        float seconds();

        std::chrono::time_point<std::chrono::high_resolution_clock>
        current_time();

    private:
        std::chrono::time_point<std::chrono::high_resolution_clock>
          m_stop_watch;
    };
};