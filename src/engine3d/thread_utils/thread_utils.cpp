#include <thread_utils/thread_utils.hpp>
#include <thread>

namespace engine3d{
    uint32_t GetThreadCount(){
        return std::thread::hardware_concurrency();
    }
};