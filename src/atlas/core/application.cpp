#include <core/application.hpp>
#include <core/engine_logger.hpp>
#include <core/event/event.hpp>
#include <imgui.h>
#include <physics/jolt-cpp/jolt-imports.hpp>
#include <physics/jolt-cpp/jolt_api.hpp>
#include <string>

#include <drivers/ui/imgui_backend.hpp>

#include <core/update_handlers/sync_update.hpp>
#include <drivers/vulkan-cpp/vk_swapchain.hpp>

namespace atlas {
    static std::string g_tag = "engine3d";
    application* application::s_instance = nullptr;
    static API g_graphics_backend_api = API::VULKAN;
    static float g_delta_time = 0.f;
    static float g_physics_step = 0.f; // collision step

    application::application(const application_settings& p_settings) {
        g_tag = p_settings.Name;
        console_log_manager::set_current_logger(g_tag);
        set_current_api(API::VULKAN);
        window_settings settings = {
            .width = p_settings.Width,
            .height = p_settings.Height,
            .name = p_settings.Name
        };
        m_window = create_window(settings);

        // renderer::initialize();
        m_renderer = create_scope<renderer>(m_window->current_swapchain(), "Renderer");
        m_renderer->set_background_color({1.f, 0.5f, 0.5f, 1.f});
        // if(m_renderer == nullptr) {
        //     console_log_trace("Renderer == NULLPTR!");
        // }
        // else {
        //     console_log_trace("Renderer != NULLPTR!");
        // }
        // // renderer
        // imgui_backend::initialize();
        s_instance = this;
    }

    uint32_t application::current_frame() {
        return s_instance->m_current_frame_index;
    }

    application::~application() {
        destroy();
    }

    void application::set_current_api(API api) {
        g_graphics_backend_api = api;
    }

    VkSwapchainKHR application::get_current_swapchain() {
        return get_window().current_swapchain();
    }

    API application::current_api() {
        return g_graphics_backend_api;
    }

    void application::destroy() {
        console_log_trace("application::destroy() called!!!");
        s_instance->get_window().close();
    }

    float application::delta_time() {
        return g_delta_time;
    }

    float application::physics_step() {
        return g_physics_step;
    }

    void application::execute() {
        // float previous_time = 0.f;
        console_log_info("Executing mainloop!");
        
        while (m_window->available()) {
            m_current_frame_index = m_window->acquired_next_frame();

            // TODO: Going to need to figure out where to put this
            // Added this here because to ensure the handlers being used by the renderer is in sync when swapchain is resized 
            vk::vk_command_buffer currently_active = m_window->active_command_buffer(m_current_frame_index);
            // VkFramebuffer current_fb = m_window->current_swapchain().active_framebuffer(m_current_frame_index);
            // VkRenderPass current_rp = m_window->current_swapchain().swapchain_renderpass();
        //     //! @brief Keeping it simply to getting our delta time
        //     //! @brief Then again, I want to have a proper fps-timer
        //     //! implementation to simplify calculating the fps time and accuracy
        //     float current_time = (float)glfwGetTime();
        //     g_delta_time = (current_time - previous_time);
        //     previous_time = current_time;

        //     // updating physic steps according to the delta time
        //     g_physics_step = 1 + (60 * g_delta_time);

            event::update_events();

        //     // m_renderer->begin();
            sync_update::on_update();

        //     sync_update::on_physics_update();

        //     sync_update::on_ui_update();

            m_renderer->begin(currently_active, m_window->current_swapchain());
            // m_renderer->end();
            m_renderer->end();

            m_window->present(m_current_frame_index);
        }
        console_log_warn("Leaving executed mainloop!");
    }

    void application::post_destroy() {
        m_window->close();
    }

    float application::aspect_ratio() {
        return s_instance->m_window->aspect_ratio();
    }

    uint32_t application::image_size() {
        return s_instance->m_window->current_swapchain().image_size();
    }
};