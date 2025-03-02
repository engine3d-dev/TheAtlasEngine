#pragma once
#include <core/engine_logger.hpp>
#include <thread>

namespace atlas {
    /**
     * @name Thread
     * @note Here are a few things we need for these dedicated threads
     * @note Because Render and Physics thread may differ here is how I see them
     * working together for the time being
     *
     * Thread Abstraction (In general should have)
     *  - Keeps track of Thread Policy (setting scheduling policy for our
     * specific threads)
     *  - Can also set prioritized policies for which thread to be scheduled
     *
     * Render Thread
     * - Handle next frame of the rendering that happens
     * - Handle in setting specific requests from the user and differentiate
     * these requests to either render or physics thread.
     */
    class thread {
    public:
        thread() = delete;
        thread(const std::string& p_tag = "Undefined")
          : m_tag(p_tag) {}

        ~thread() {
            console_log_info("Are you destructed!");
            if (this->Joinable()) {
                this->Join();
            }
        }

        template<typename T, typename... Args>
        void dispatch(T&& func, Args&&... args) {
            m_thread = std::thread(func, std::forward<Args>(args)...);
        }

        bool Joinable() { return m_thread.joinable(); }

        void Join() { m_thread.join(); }

    private:
        std::string m_tag;
        std::thread m_thread;
    };
};