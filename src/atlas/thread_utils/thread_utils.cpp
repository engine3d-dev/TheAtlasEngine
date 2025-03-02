#include <thread>
#include <thread_utils/thread_utils.hpp>


namespace atlas {
    uint32_t get_thread_count() {
        return std::thread::hardware_concurrency();
    }
};