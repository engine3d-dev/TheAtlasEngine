#include "Core/EngineLogger.h"
#include "Core/UI/UICore.h"
#include <Core/backend/internal/FrameTimer.h>
#include <Core/ApplicationInstance.h>
#include <Core/Renderer/Renderer.h>
#include <Core/Timestep.h>
#include <Core/backend/Vulkan/Vulkan.h>
#include <GLFW/glfw3.h>

namespace engine3d{
    static float m_LastFrameTime = 0.0f;
    static std::string g_DebugName = "Engine3D";

    ApplicationInstance::ApplicationInstance(const std::string& p_DebugName){
        g_DebugName = p_DebugName;
    }

    void ApplicationInstance::StartApplicationRunMainloop(){
        ConsoleLogInfo("EditorApplication::RunApplicationMainloop called!");

        while(!glfwWindowShouldClose(vk::VulkanPipeline::GetCurrentWindow())){
            //! @note 
            FrameTimer::UpdateFrameTimer();
            glfwPollEvents();

            // UICore::BeginImguiFrame();
            UpdateCurrentApplicationInstance();
            // UICore::EndImguiFrame();
        }
        
        //! @note Cleaning up imgui
        UICore::CleanupImgui();
    }

    void ApplicationInstance::UpdateCurrentApplicationInstance(){
        UpdateThisApplicationInstance();
    }

    float ApplicationInstance::CurrentFrameTime(){
        return m_LastFrameTime;
    }

    std::string& ApplicationInstance::CurrentApplicationName(){
        return g_DebugName;
    }
};