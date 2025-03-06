#pragma once
#include <string>

namespace atlas {
    /**
     * @note Current work to get back to it.
     * 
     * 
     * @note The thought we are doing are the following
     * 
     * @note First implement thread.hpp
     * @note Implementing the thread class to wrap around the <thread> for handling and doing thread stuff like thread statuses, policies, etc
     * 
     * @note Second improve <thread_pool> to first be able to spawn those threads
     * @note Then once we can spawn those specific threads successfully then we apply what global update does and use a similar logic into the <thread_pool> itself
     * 
     * @note Third, fix up the FPS and timer for udpate frequency
     * 
    */
    class thread{
    public:
        thread(const std::string& p_tag);

        template<typename T>
        void dispatch([[maybe_unused]] const T& p_function) {}
    };
};