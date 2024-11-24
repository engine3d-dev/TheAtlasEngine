#include "Renderer/Renderer.hpp"
// #include "TimeManagement/GlobalUpdateManager.hpp"
#include <Core/Event/InputPoll.hpp>
#include <Core/ApplicationInstance.hpp>
#include <Core/Timestep.hpp>
#include <Core/EngineLogger.hpp>
#include <entt/entt.hpp>

// #include <imgui/backends/imgui_impl_vulkan.h>
#include <imgui/backends/imgui_impl_glfw.h>

namespace engine3d{
    static float m_LastFrameTime = 0.0f;
    static std::string g_DebugName = "Engine3D";
    static API g_CurrentAPI = UNSPECIFIED;

    ApplicationInstance* ApplicationInstance::g_ThisInstance = nullptr;

    ApplicationInstance::ApplicationInstance(const std::string& p_DebugName){
        g_ThisInstance = this;
        g_DebugName = p_DebugName;
        SetCurrentAPI(VULKAN);
        m_Window = Window::Create(1280, 720, p_DebugName);

        // ImGui_ImplVulkanH_Window();
        // ImGui_ImplGlfw_InitForVulkan(ApplicationInstance::GetWindow().GetNativeWindow(), true);
        Renderer::Initialize("Engine3D Renderer");
    }

    void ApplicationInstance::ExecuteApplicationMainloop(){
        ConsoleLogInfo("EditorApplication::RunApplicationMainloop called!");

        while(m_Window->IsActive()){
            InputPoll::UpdateEvents();
            // FrameTimer::UpdateFrameTimer(); // give us the frames in flight.

            Renderer::BeginFrame();
            m_Window->OnUpdateAllFrames();
            Renderer::EndFrame();
        }
        
        //! @note Cleaning up imgui
        // UICore::CleanupImgui();
    }

    ApplicationInstance& ApplicationInstance::Super(){ return *g_ThisInstance; }

    // void ApplicationInstance::UpdateCurrentApplicationInstance(){
    //     UpdateThisApplicationInstance();
    // }

    float ApplicationInstance::CurrentFrameTime(){
        return m_LastFrameTime;
    }

    std::string& ApplicationInstance::CurrentApplicationName(){
        return g_DebugName;
    }

    void ApplicationInstance::SetCurrentAPI(API p_CurrentApi){
        g_CurrentAPI = p_CurrentApi;
    }

    API ApplicationInstance::CurrentAPI(){
        return g_CurrentAPI;
    }

    Window& ApplicationInstance::GetWindow() { return *g_ThisInstance->m_Window; }
};
