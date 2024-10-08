// #include <Core/internal/FrameTimer.h>
#include <Core/ApplicationInstance.h>
#include <Core/Timestep.h>
// #include <Core/Renderer/Renderer.h>
#include <Core/EngineLogger.h>

namespace engine3d{
    static float m_LastFrameTime = 0.0f;
    static std::string g_DebugName = "Engine3D";
    static API g_CurrentAPI = UNSPECIFIED;
    ApplicationInstance* ApplicationInstance::g_ThisInstance = nullptr;

    ApplicationInstance::ApplicationInstance(const std::string& p_DebugName){
        g_ThisInstance = this;
        g_DebugName = p_DebugName;
        SetCurrentAPI(VULKAN);
        m_Window = Window::Create();
    }

    void ApplicationInstance::ExecuteApplicationMainloop(){
        ConsoleLogInfo("EditorApplication::RunApplicationMainloop called!");

        while(m_Window->IsActive()){
            // FrameTimer::UpdateFrameTimer(); // give us the frames in flight.

            // Renderer::Presentation();
            m_Window->OnUpdatePerTick();
        }
        
        //! @note Cleaning up imgui
        // UICore::CleanupImgui();
    }

    ApplicationInstance& ApplicationInstance::Super(){ return *g_ThisInstance; }

    void ApplicationInstance::UpdateCurrentApplicationInstance(){
        UpdateThisApplicationInstance();
    }

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