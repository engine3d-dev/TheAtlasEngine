#include <core/application.hpp>
#include <core/engine_logger.hpp>
#include <core/event/event.hpp>
#include <core/update_handlers/global_update.hpp>
#include <imgui.h>
#include <physics/jolt-cpp/jolt-imports.hpp>
#include <physics/jolt-cpp/jolt_api.hpp>
#include <string>

#include <renderer/renderer.hpp>
#include <drivers/ui/imgui_backend.hpp>

#include <core/update_handlers/sync_update.hpp>
#include <core/timer.hpp>
#include <chrono>

namespace atlas {
    static std::string g_tag = "engine3d";
    application* application::s_instance = nullptr;
    static API g_graphics_backend_api = API::UNSPECIFIED;

    application::application(const application_settings& p_settings) : m_sync_update_thread("sync update") {
        s_instance = this;
        g_tag = p_settings.Name;
        console_log_manager::set_current_logger(g_tag);
        set_current_api(API::VULKAN);
        m_window = window::create(p_settings.Width, p_settings.Height, g_tag);

        renderer::initialize();
        ImGuiBackend::Initialize();
    }

    application::~application() {
        destroy();
    }

    void application::set_current_api(API api) {
        g_graphics_backend_api = api;
    }

    ref<swapchain> application::get_current_swapchain() {
        return get_window().get_current_swapchain();
    }

    API application::current_api() {
        return g_graphics_backend_api;
    }

    void application::destroy() {
        s_instance->get_window().close();
    }

    // static float s_current_delta_time = 0.0f;
    static std::chrono::time_point<std::chrono::high_resolution_clock>
      s_updatetimer;
    timer s_clock_timer;
    timer s_fps_timer;
    static float s_max_fps = 0.0f;
    static float s_fps_counter = 0.0f;
    // static constexpr int seconds = 1000000;
    float g_delta_time = 0.0f;


    // sync update variables
    static timer s_localtimer = timer();
    // static std::chrono::time_point<std::chrono::high_resolution_clock>
    //   s_local_update_time;
    static float s_current_delta_time = 0.f;
    static float s_delta_time = 0.0f;
    static float s_previous_delta_time = 0.f;

    // static float s_sync_global_delta_time = 0.0f;
    // static int s_max_variance = 0;
    // static int s_min_frames = 0;
    // static int s_local_update_counter = 0;
    // static int s_local_framerate_per_second = 0;
    // static int s_random_frame;

    float application::delta_time() {
        return s_delta_time;
    }

    void application::execute() {
        s_clock_timer = timer();
        s_fps_timer = timer();
        s_updatetimer = s_clock_timer.current_time();
        s_max_fps = 100;
        s_fps_counter = 1;

        // sync update variable initialization
        // s_localtimer = timer();
        // s_local_update_time = s_localtimer.current_time();
        // s_sync_local_delta_time = 0.0;
        // s_max_variance = 2;
        // s_min_frames = 0;
        // s_local_update_counter = 0;
        // s_local_framerate_per_second = 0;
        // srand(std::time(nullptr));
        // s_random_frame = (rand() % s_max_variance) + s_min_frames;

        console_log_info("Executing mainloop!");

        while (m_window->is_active()) {
            s_current_delta_time = (float)glfwGetTime();
            s_delta_time = s_current_delta_time - s_previous_delta_time;
            s_previous_delta_time = s_current_delta_time;
            // s_delta_time /= seconds;
            event::update_events();

            //! @note This was used for getting the delta update time
            // s_current_delta_time =
            // std::chrono::duration_cast<std::chrono::microseconds>(
            //     s_clock_timer.current_time() - s_updatetimer)
            //     .count();
            // s_updatetimer = s_clock_timer.current_time();
            // g_delta_time = s_current_delta_time / seconds;

            renderer::begin();
            sync_update::on_update();

            sync_update::on_physics_update();
            time::update_physics_step();

            sync_update::on_ui_update();

            renderer::end();
            
            // sync_update::run_update(g_delta_time);
            /*
            s_sync_global_delta_time = s_current_delta_time;
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
            */
        }
        console_log_warn("Leaving executed mainloop!");
    }

    uint32_t application::get_aspect_ratio() {
        return get_window().get_width() / get_window().get_height();
    }
};