#include "drivers/ui/imgui_backend.hpp"
#include "renderer/renderer.hpp"
#include <core/application_instance.hpp>
#include <core/engine_logger.hpp>
#include <core/event/input_poll.hpp>
#include <core/update_handlers/global_update_manager.hpp>
#include <string>

namespace engine3d{
    static std::string g_Tag = "engine3d";
    ApplicationInstance* ApplicationInstance::s_ThisInstance = nullptr;
    static API g_GraphicBackendAPI = API::UNSPECIFIED;

    ApplicationInstance::ApplicationInstance(const std::string& p_Tag){
        s_ThisInstance = this;
        g_Tag = p_Tag;
        ConsoleEngineLogger::SetCurrentApplicationTagLogger(g_Tag);
        SetCurrentAPI(API::VULKAN);
        m_Window = Window::Create(900, 600, g_Tag);
        
        Renderer::Initialize();
        // ImGuiBackend::Initialize();
    }

    void ApplicationInstance::SetCurrentAPI(API api){
        g_GraphicBackendAPI = api;
    }

    Ref<Swapchain> ApplicationInstance::GetCurrentSwapchani(){
        return GetWindow().GetCurrentSwapchain();
    }

    API ApplicationInstance::CurrentAPI(){
        return g_GraphicBackendAPI;
    }

    void ApplicationInstance::ShutdownApplication(){
        Super()->GetWindow().Close();
    }

    void ApplicationInstance::ExecuteMainloop(){
        ConsoleLogInfo("Executing mainloop!");

        while(m_Window->IsWindowActive()){
            InputPoll::Update();

            Renderer::Begin();
            GlobalUpdateManager::GlobalOnTickUpdate();
            Renderer::End();

            // ImGuiBackend::Begin();
            // ImGuiBackend::End();
        }
        ConsoleLogWarn("Leaving executed mainloop!");
    }
};