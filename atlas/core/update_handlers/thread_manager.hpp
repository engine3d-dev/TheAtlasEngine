#pragma once
#include <core/core.hpp>
#include <thread_utils/thread.hpp>
#include <vector>

namespace atlas{
    /**
     * @name Thread Manager
     * @note Might get replaced by task graph since this allows us to synchronously work with different tasks while still being in parallel
     * @note Here are the purpose thread pools should be handling for the time being.API
     * @note 1.) Lifetime of threads (render, physics, audio)
    */
    class thread_manager{
    public:
        thread_manager();
        ~thread_manager();

        void on_run(float p_delta_time);
        void on_stop();

        // static uint32_t GetMaxThreads();
    private:
        void update_sync_function();
        void update_parallel_function();
    private:
        bool m_thread_stop = false;
        thread m_sync_update_thread;
        // Thread m_render_thread;

        //! @note Threads specifically for parallel
        std::vector<scope<thread>> m_parallel_threads;
        float m_delta_time = 0.0f;
    };
};