
#include <chrono>
namespace engine3d{
    class Timer{

        public:
        Timer();

        void reset();

        float elapsed();

        float elapsedSec();

        std::chrono::time_point<std::chrono::high_resolution_clock> getCurrentTime();

        private:
            std::chrono::time_point<std::chrono::high_resolution_clock> m_stopWatch;

    };
};