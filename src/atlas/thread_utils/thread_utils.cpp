#include <thread_utils/thread_utils.hpp>
#include <thread>

namespace atlas{
    uint32_t get_thread_count(){
        return std::thread::hardware_concurrency();
    }
};