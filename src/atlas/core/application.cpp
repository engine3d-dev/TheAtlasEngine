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

namespace atlas {
    static std::string g_tag = "engine3d";
    application* application::s_instance = nullptr;
    static API g_graphics_backend_api = API::UNSPECIFIED;

    application::application(
      const application_settings& p_settings) {
        s_instance = this;
        g_tag = p_settings.Name;
        console_log_manager::set_current_logger(g_tag);
        set_current_api(API::VULKAN);
        m_window = window::create(p_settings.Width, p_settings.Height, g_tag);

        renderer::initialize();
        ImGuiBackend::Initialize();

        physics::JoltAPI::InitializeJoltAPI();
    }

    application::~application(){
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

    void application::execute() {
        console_log_info("Executing mainloop!");

        while (m_window->is_active()) {
            event::update_events();

            renderer::begin();
            global_update::tick_update_frequency();

            //! TODO: Submit ImGuiBackend::Begin so we can have the UI also be
            //! on the renderer thread
            //! @note So we can make sure when submitting UI-stuff are in synced
            //! with the renderer in terms of fetching images
            //! TODO: ImGuiBackend::Begin/End() is needed be called after
            //! everything gets rendered.
            //! TODO: UI gets rendered last
            // ImGuiBackend::Begin();
            // if(ImGui::Begin("Setting")){
            //     ImGui::Button("Pres Me!");
            //     ImGui::End();
            // }
            // ImGuiBackend::End();

            // global_update::UpdateUI();

            // SyncUpdate::on_ui_update();

            renderer::end();
        }
        console_log_warn("Leaving executed mainloop!");
    }

    uint32_t application::get_aspect_ratio() {
        return get_window().get_width() / get_window().get_height();
    }
};