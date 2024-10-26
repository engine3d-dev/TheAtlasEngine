#include <Core/TimeManagement/Timer.hpp>
#include <chrono>

using namespace std::chrono; 
namespace engine3d
{

    Timer::Timer()
    {
        Reset();
    }

    // set a start time
    void Timer::Reset()
    {
        m_StopWatch = high_resolution_clock::now();
    }

    // Get current Time
    time_point<high_resolution_clock> Timer::GetCurrentTime()
    {
        return high_resolution_clock::now();
    }

    float Timer::Elapsed()
    {
        return duration_cast<microseconds>(GetCurrentTime()-m_StopWatch).count();
    }

    float Timer::ElapsedSec()
    {
        return Elapsed()/1000000;
    }
}