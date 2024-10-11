#include <Core/TimeManagement/Timer.hpp>
#include <chrono>

using namespace std::chrono; 
namespace engine3d
{

    Timer::Timer()
    {
        reset();
    }

    // set a start time
    void Timer::reset()
    {
        m_stopWatch = high_resolution_clock::now();
    }

    // Get current Time
    time_point<high_resolution_clock> Timer::getCurrentTime()
    {
        return high_resolution_clock::now();
    }

    float Timer::elapsed()
    {
        return duration_cast<microseconds>(getCurrentTime()-m_stopWatch).count();
    }

    float Timer::elapsedSec()
    {
        return elapsed()/1000000;
    }
}