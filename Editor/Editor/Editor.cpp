#include "Editor.h"
#include "EngineLayer.h"
#include "UILayer.h"
#include <engine3d/Core/Timestep.h>
#include <GLFW/glfw3.h>

namespace engine3d{

    EditorApplication::EditorApplication(){
        InitiateEditor();
    }

    EditorApplication::~EditorApplication(){
        ShutdownEditor();
    }

    void EditorApplication::InitiateEditor(){
        EngineLogger::Initialize();
        m_Pipeline.InitializeVulkan();
        m_VulkDevice.InitializeVulkanDevices();

        //! @note Initially we will have our actual rendering and everything engine-related in EngineLayer
        //! @note UILayer is where all of the UI-related stuff are done.
        //! @note UILayer for right now is used for initiating any imgui related configurations.
        m_Layers[0] = new EngineLayer();
        m_Layers[0]->OnAttach();
        m_Layers[1] = new UILayer();
        m_Layers[1]->OnAttach();
    }

    void EditorApplication::ShutdownEditor(){
        m_Pipeline.CleanVulkanPipeline();
        m_VulkDevice.CleanupVulkanDevice();
    }

    void EditorApplication::RunEditor(){

        while(!glfwWindowShouldClose(VulkanPipeline::GetCurrentWindow())){
            m_LastFrameTime = (float)glfwGetTime();
            Timerstep ts = m_LastFrameTime;
            // m_Layers[0]->OnUpdate(ts);

            for(const auto& layer : m_Layers){
                layer->OnUpdate(ts);
            }

            for(const auto& layer : m_Layers){
                layer->OnUIRender();
            }

            glfwPollEvents();
        }
    }

};