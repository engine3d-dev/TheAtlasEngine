#include <core/update_handlers/timer.hpp>
#include <chrono>
using namespace std::chrono;

namespace atlas{

    timer::timer(){
        reset();
    }

    // set a start time
    void timer::reset(){
        m_stop_watch = high_resolution_clock::now();
    }

    // Get current Time
    time_point<high_resolution_clock> timer::current_time(){
        return high_resolution_clock::now();
    }

    float timer::elapsed(){
        return duration_cast<microseconds>(current_time()-m_stop_watch).count();
    }

    float timer::seconds(){
        return elapsed()/1000000;
    }
}