#include <chrono>
#include <core/application.hpp>
#include <core/engine_logger.hpp>
#include <core/event/event.hpp>
#include <core/timer.hpp>
#include <core/update_handlers/global_update.hpp>
#include <core/update_handlers/sync_update.hpp>

namespace atlas {
    static float s_current_delta_time = 0.0f;
    scope<thread_manager> s_thread_manager = nullptr;
    static std::chrono::time_point<std::chrono::high_resolution_clock>
      s_Updatetimer;
    timer s_clock_timer;
    timer s_fps_timer;
    static float s_max_fps = 0.0f;
    static float s_fps_counter = 0.0f;
    static constexpr int SECONDS = 1000000;
    float g_delta_time = 0.0f;
    // static scope<std::thread> s_render_thread = nullptr;

    // this needs to be called every frame?
    // std::atomic<uint32_t> g_ThreadCounter;
    // std::mutex g_GlobalLock;
    // std::condition_variable g_GlobalConditional;

    // Not Needed
    // std::deque<std::function<void()>> global_update::s_ApplicationUpdates;

    void global_update::initialize() {
        s_clock_timer = timer();
        s_fps_timer = timer();
        s_thread_manager = create_scope<thread_manager>();

        // s_SyncUpdateManager = create_scope<SyncUpdate>();
        sync_update::initialize();

        s_current_delta_time = 0.0f;
        s_Updatetimer = s_clock_timer.current_time();
        s_max_fps = 100;
        s_fps_counter = 1;
        // s_render_thread = create_scope<std::thread>();
        // s_ApplicationUpdates = std::deque<std::function<void()>>();
        console_log_info("F1 to see global time and F2 to see local time");
    }

    void global_update::tick_update_frequency() {
        s_fps_timer.reset();

        //! @note Physics, since calculations must be fast before we render.
        //! This is how we are going to do this for right now
        //! @note TODO: Need to have better ways of accessing runtime physics
        //! integrated into the engine
        sync_update::on_physics_update();
        time::update_physics_step();

        sync_update::on_update();

        if (s_clock_timer.seconds() >= 1) {
            // @note late frame in order to squeeze input frames with

            if (event::is_key_pressed(KeyCode::F1)) {
                console_log_info("FPS: {1} Delta Time: {0}",
                                 s_current_delta_time,
                                 s_fps_counter);
            }

            // @note late frame in order to squeeze input frames with
            // SyncUpdate::on_update();

            s_clock_timer.reset();
            s_fps_counter = 1;
        }
        else {
            s_fps_counter++;
        }

        sync_update::on_ui_update();

        // sync_update::on_update();
        sync_update::on_scene_render();
        // std::thread render_thread();
        // s_render_thread =
        // create_scope<std::thread>(sync_update::on_scene_render);

        // sync_update::on_ui_update();
        s_current_delta_time =
          std::chrono::duration_cast<std::chrono::microseconds>(
            s_clock_timer.current_time() - s_Updatetimer)
            .count();
        s_Updatetimer = s_clock_timer.current_time();
        g_delta_time = s_current_delta_time / SECONDS;

        s_thread_manager->on_run(g_delta_time);
        // SyncUpdate::run_update(g_delta_time);
        wait_next_frame();
    }

    void global_update::IncrementCounter() {
        // g_ThreadCounter |= 1;
    }

    // Maintains a const fps if possible
    void global_update::wait_next_frame() {

        while (s_fps_timer.seconds() < 1.0f / s_max_fps) {
            continue;
        }

        // g_ThreadCounter += 1;
        // std::unique_lock<std::mutex> m(g_GlobalLock);
        // g_GlobalConditional.wait(m, [](){});

        // while(g_ThreadCounter < (get_thread_count()/2) + 1){
        //     continue;
        // }

        // std::this_thread::sleep_for(std::chrono::duration<double,
        // std::milli>(1.0f/s_MaxFrameratePerSecond -
        // s_fps_timer.ElapsedSec()));
    }

    void global_update::cleanup() {
        console_log_warn("Global Update Manager is cleaning up!");
        s_thread_manager->on_stop();
    }
};