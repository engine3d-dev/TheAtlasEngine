#include <barrier>
#include <condition_variable>
#include <core/engine_logger.hpp>
#include <core/update_handlers/thread_manager.hpp>
#include <fmt/format.h>
#include <thread_utils/thread_utils.hpp>
#include <update_handlers/global_update.hpp>
#include <update_handlers/sync_update.hpp>

namespace atlas {
    /*
    std::mutex g_sync_lock;
    std::mutex g_parallel_lock;
    //! @note conditional variable specifically for SyncUpdateManager's thread.
    //! @note Name's or use may change later for thread manager. Since
    //! TaskGraph/Signal trees WILL replace this.
    std::condition_variable g_sync_frame;
    std::condition_variable g_parallel_frame;

    static bool g_ready = false;
    static bool g_ready2 = false;

    std::atomic<uint32_t> g_ParallelCounter;

    thread_manager::thread_manager()
      : m_sync_update_thread("Sync Update Thread") {
        // thread_manager::thread_manager(){
        m_thread_stop = false;
        m_sync_update_thread.dispatch(&thread_manager::update_sync_function,
                                      this);
        // m_render_thread.dispatch(&sync_update::on_scene_render);
        //! @note Allocating the threads needed for the parallel frame to run at

        // if(syncUpdateThread.Joinable()){
        //     console_log_info("Thread Manager is running!");
        // }

        // if(m_render_thread.Joinable()){
        //     console_log_info("Render thread still running!!!");
        // }

        // auto core_amount_half = get_thread_count() / 2;
        // console_log_trace("Amount of Threads Specified: {}",
        // core_amount_half);

        // Commenting and will either git pull or implement this if zach hasn't
        // done so, already for(uint32_t i = 0; i < core_amount_half; i++){
        //     Thread t = Thread(fmt::format("Thread {}", i));
        //     t.dispatch(&thread_manager::update_parallel_function, this);
        //     m_parallel_threads.push_back(&t);
        // }
    }

    thread_manager::~thread_manager() {
        m_thread_stop = true;
        g_ready = true;
        g_ready2 = true;
        g_sync_frame.notify_all();
        g_parallel_frame.notify_all();
    }

    void thread_manager::on_run(float p_delta_time) {
        m_delta_time = p_delta_time;
        g_ready = true;
        g_ready2 = true;
        g_sync_frame.notify_one();
        // g_parallel_frame.notify_all();
    }

    void thread_manager::update_sync_function() {
        while (!m_thread_stop) {
            std::unique_lock<std::mutex> m(g_sync_lock);

            // global_update::IncrementCounter();

            g_sync_frame.wait(m, [] { return g_ready; });

            if (m_thread_stop) {
                console_log_info("Cleaning up Thread");
                break;
            }

            g_ready = false;
            sync_update::run_update(m_delta_time);
        }
    }

    void thread_manager::update_parallel_function() {
        //! @note 1.) Get all threads to wait on g_sync_frame

        console_log_info("Are you running?");
        while (!m_thread_stop) {
            // 1.)
            std::barrier sync_point(m_parallel_threads.size(), [&]() noexcept {
                console_log_info("Inside Barrier");
            });

            global_update::IncrementCounter();

            g_ParallelCounter += 1;
            std::unique_lock<std::mutex> m(g_parallel_lock);

            g_parallel_frame.wait(m, [] { return g_ready2; });

            std::barrier sync_point2(m_parallel_threads.size(), [&]() noexcept {
                console_log_info("Inside Barrier #2");
            });

            if (m_thread_stop) {
                console_log_info("Cleaning up Thread Parallel");
                break;
            }

            g_ready2 = false;
            sync_update::run_update(m_delta_time);
        }

        //! @note 2.)
    }

    void thread_manager::on_stop() {
        m_thread_stop = true;
        g_ready = true;
        g_ready2 = true;
        g_sync_frame.notify_all();
        // g_parallel_frame.notify_all();
    }
    */
};