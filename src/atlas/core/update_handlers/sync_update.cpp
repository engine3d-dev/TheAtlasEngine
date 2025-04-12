#include <core/engine_logger.hpp>
#include <core/event/event.hpp>
#include <core/update_handlers/sync_update.hpp>

namespace atlas::sync_update {
    /*
    static timer s_localtimer = timer();
    static std::chrono::time_point<std::chrono::high_resolution_clock>
      s_local_update_time;
    static float s_sync_local_delta_time = 0.0f;
    static float s_sync_global_delta_time = 0.0f;
    static int s_max_variance = 0;
    static int s_min_frames = 0;
    static int s_local_update_counter = 0;
    static int s_local_framerate_per_second = 0;
    static int s_random_frame;

    void initialize() {
        console_log_info("InitializeSyncUpdate Initialized!!");
        s_localtimer = timer();
        s_local_update_time = s_localtimer.current_time();
        s_sync_local_delta_time = 0.0;
        s_max_variance = 2;
        s_min_frames = 0;
        s_local_update_counter = 0;
        s_local_framerate_per_second = 0;
        srand(std::time(nullptr));
        s_random_frame = (rand() % s_max_variance) + s_min_frames;
    }

    //! @note this does not work per object this might need to change a little.
    //! Possibly pass gameObjects with the virtual functions.
    //! Possibly seperate active scripts to non active ones in scenes.
    void run_update(float delta_time) {

        s_sync_global_delta_time = delta_time;
        // on_physics_update();
        // const int collisionSteps = 1 + (60*(deltaTime));


        if (s_random_frame <= s_local_update_counter) {
            s_random_frame = (rand() % s_max_variance) + s_min_frames;
            s_local_update_counter = 0;

            // on_update();
            // on_late_update();

            s_sync_local_delta_time =
              duration_cast<std::chrono::microseconds>(
                s_localtimer.current_time() - s_local_update_time)
                .count();
            s_sync_local_delta_time = s_sync_local_delta_time / 1000000;

            s_local_update_time = s_localtimer.current_time();
            s_local_framerate_per_second++;
        }
        else {
            s_local_update_counter++;
        }

        //! @note I don't understand why this works but localtime doesn't.
        //! @note My guess is that it needs to be called every frame/ double
        //! buffering might be an issue.
        // on_scene_render();
        if (s_localtimer.seconds() >= 1.0) {
            s_localtimer.reset();

            //! @note Key event added to allow switch between global and local.
            if (event::is_key_pressed(KeyCode::K)) {
                console_log_info("Local FPS: {0}, Local Delta Time: {1}",
                                 s_local_framerate_per_second,
                                 s_sync_local_delta_time);
            }
            s_local_framerate_per_second = 0;
        }
    }

    float delta_time() {
        return s_sync_local_delta_time;
    }
    */

    void on_update() {
        for (auto& l_subscriber : s_Update) {
            l_subscriber();
        }
    }

    void on_ui_update() {

        for (auto& ui_update : s_UIUpdate) {
            ui_update();
        }
    }

    void on_late_update() {
        for (auto& l_subscriber : s_LateUpdate) {
            l_subscriber();
        }
    }

    void on_physics_update() {
        for (auto& l_subscriber : s_PhysicsQueue) {
            l_subscriber();
        }
    }

};