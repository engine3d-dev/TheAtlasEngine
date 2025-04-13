#include <core/application.hpp>
#include <core/engine_logger.hpp>
#include <core/event/event.hpp>
#include <imgui.h>
#include <physics/jolt-cpp/jolt-imports.hpp>
#include <physics/jolt-cpp/jolt_api.hpp>
#include <string>

#include <renderer/renderer.hpp>
#include <drivers/ui/imgui_backend.hpp>

#include <core/update_handlers/sync_update.hpp>

namespace atlas {
    static std::string g_tag = "engine3d";
    application* application::s_instance = nullptr;
    static API g_graphics_backend_api = API::UNSPECIFIED;
    static float g_delta_time = 0.f;
    static float g_physics_step = 0.f; // collision step

    application::application(const application_settings& p_settings) {
        s_instance = this;
        g_tag = p_settings.Name;
        console_log_manager::set_current_logger(g_tag);
        set_current_api(API::VULKAN);
        m_window = window::create(p_settings.Width, p_settings.Height, g_tag);

        renderer::initialize();
        imgui_backend::initialize();
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

    float application::delta_time() {
        return g_delta_time;
    }

    float application::physics_step() {
        return g_physics_step;
    }

    void application::execute() {
        float previous_time = 0.f;
        console_log_info("Executing mainloop!");

        while (m_window->is_active()) {
            //! @brief Keeping it simply to getting our delta time
            //! @brief Then again, I want to have a proper fps-timer
            //! implementation to simplify calculating the fps time and accuracy
            float current_time = (float)glfwGetTime();
            g_delta_time = (current_time - previous_time);
            previous_time = current_time;

            // updating physic steps according to the delta time
            g_physics_step = 1 + (60 * g_delta_time);

            event::update_events();

            renderer::begin();
            sync_update::on_update();

            sync_update::on_physics_update();

            sync_update::on_ui_update();

            renderer::end();
        }
        console_log_warn("Leaving executed mainloop!");
    }

    uint32_t application::get_aspect_ratio() {
        return get_window().get_width() / get_window().get_height();
    }
};