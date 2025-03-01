#include "drivers/ui/imgui_backend.hpp"
#include "renderer/renderer.hpp"
#include <core/application_instance.hpp>
#include <core/engine_logger.hpp>
#include <core/event/event.hpp>
#include <core/update_handlers/global_update.hpp>
#include <imgui.h>
#include <string>
#include <physics/jolt-cpp/jolt-imports.hpp>
#include <physics/jolt-cpp/jolt_api.hpp>

namespace atlas{
    static std::string g_tag = "engine3d";
    application_instance* application_instance::s_ThisInstance = nullptr;
    static API g_GraphicBackendAPI = API::UNSPECIFIED;

    application_instance::application_instance(const application_settings& p_settings){
        s_ThisInstance = this;
        g_tag = p_settings.Name;
        console_log_manager::set_current_logger(g_tag);
        set_current_api(API::VULKAN);
        m_Window = window::create(p_settings.Width, p_settings.Height, g_tag);
        
        renderer::initialize();
        ImGuiBackend::Initialize();

        physics::JoltAPI::InitializeJoltAPI();

        // physics::PhysicSystem physics_system = physics::PhysicSystem();
    }

    void application_instance::set_current_api(API api){
        g_GraphicBackendAPI = api;
    }

    ref<swapchain> application_instance::get_current_swapchain(){
        return get_window().get_current_swapchain();
    }

    API application_instance::current_api(){
        return g_GraphicBackendAPI;
    }

    void application_instance::ShutdownApplication(){
        super()->get_window().close();
    }

    void application_instance::execute(){
        console_log_info("Executing mainloop!");

        while(m_Window->is_active()){
            event::update_events();
            
            renderer::begin();
            global_update::tick_update_frequency();

            //! TODO: Submit ImGuiBackend::Begin so we can have the UI also be on the renderer thread
            //! @note So we can make sure when submitting UI-stuff are in synced with the renderer in terms of fetching images
            //! TODO: ImGuiBackend::Begin/End() is needed be called after everything gets rendered.
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

    uint32_t application_instance::get_aspect_ratio(){
        return get_window().get_width() / get_window().get_height();
    }
};