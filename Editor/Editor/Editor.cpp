#include "Editor.h"
#include "EngineLayer.h"
#include "UILayer.h"
#include "engine3d/Core/backend/Vulkan/VulkanCommandBuffer.h"
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
        m_Pipeline.InitializePipeline();
        m_VulkDevice.InitializeDevice();
        m_Swapchain.InitializeSwaphchain();
        m_CmdBuffer = vk::VulkanCommandBuffer(vk::VulkanSwapchain::GetImagesSize());
        m_CmdBuffer.RecordCommandBuffers();

        //! @note zero - indicates getting first data in queue
        m_CmdQueue = vk::VulkanCommandQueue(0);


        //! @note Initially we will have our actual rendering and everything engine-related in EngineLayer
        //! @note UILayer is where all of the UI-related stuff are done.
        //! @note UILayer for right now is used for initiating any imgui related configurations.
        m_Layers[0] = new EngineLayer();
        m_Layers[0]->OnAttach();
        m_Layers[1] = new UILayer();
        m_Layers[1]->OnAttach();
    }

    void EditorApplication::ShutdownEditor(){
        m_Pipeline.CleanupPipeline();
        m_VulkDevice.CleanupDevice();
        // m_Swapchain.CleanupSwapchain();
    }

    void EditorApplication::RunEditor(){

        while(!glfwWindowShouldClose(vk::VulkanPipeline::GetCurrentWindow())){
            m_LastFrameTime = (float)glfwGetTime();
            Timerstep ts = m_LastFrameTime;

            for(const auto& layer : m_Layers){
                layer->OnUpdate(ts);
            }

            // Clearing screen
            uint32_t idx = m_CmdQueue.AcquireNextImage();
            m_CmdQueue.SubmitAsync(m_CmdBuffer[idx]);
            m_CmdQueue.Presentation(idx);
            m_CmdQueue.WaitIdle();

            for(const auto& layer : m_Layers){
                layer->OnUIRender();
            }

            glfwPollEvents();
        }
    }

};