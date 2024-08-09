#include "Editor.h"
#include "EngineLayer.h"
#include "UILayer.h"
#include "engine3d/Core/Renderer/Renderer.h"
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
        //! @note Initially we will have our actual rendering and everything engine-related in EngineLayer
        //! @note UILayer is where all of the UI-related stuff are done.
        //! @note UILayer for right now is used for initiating any imgui related configurations.
        m_Layers[0] = new EngineLayer();
        m_Layers[0]->OnAttach();
        m_Layers[1] = new UILayer();
        m_Layers[1]->OnAttach();
        
        Renderer::Initialize();

        //! @note Setting our color to being light blue 
        Renderer::SetBackgroundColor({0.0f, 0.5f, 0.5f, 0.f});
        // Renderer::SetBackgroundColor({1.0f, 0.0f, 0.0f, 1.0f});

        // m_CmdBuffer = vk::VulkanCommandBuffer(vk::VulkanSwapchain::GetImagesSize());
        // m_CmdBuffer.RecordCommandBuffers();

        //! @param index, zero - indicates getting first data in queue at index 0 (zero)
        //! @note TODO -- for cmd buffers/queues, shader modules, and render passes. Figuring out what parameters are important that users should specify when creating this instance
        //! @note This is because as it stands the API requires for instancing the object before initializing it, but should just utilize the constructor to cleanup the API for initiating our object's lifetimes.
        // m_CmdQueue = vk::VulkanCommandQueue(0);
        // m_RenderPass.InitializeRenderPass();
        // m_RenderPass.InitializeFramebuffers();
        // m_TriangleShaderVertModule = vk::VulkanShaderModule("Resources/shaders/TriangleShader/triangle.vert.spirv");
        // m_TriangleShaderFragModule = vk::VulkanShaderModule("Resources/shaders/TriangleShader/triangle.frag.spirv");
        // engine3d::UILayer::InitializeUI(m_CmdQueue.GetVkQueueInstance());

        // m_ShaderPipeline = vk::VulkanShaderCompiler(m_TriangleShaderVertModule.GetVkShaderModuleInstance(), m_TriangleShaderFragModule.GetVkShaderModuleInstance(), m_RenderPass.GetRenderPassInstnace());

    }

    void EditorApplication::ShutdownEditor(){
        // m_Pipeline.CleanupPipeline();
        // m_VulkDevice.CleanupDevice();
    }

    void EditorApplication::RunEditor(){

        while(!glfwWindowShouldClose(vk::VulkanPipeline::GetCurrentWindow())){
            m_LastFrameTime = (float)glfwGetTime();
            Timerstep ts = m_LastFrameTime;

            for(const auto& layer : m_Layers){
                layer->OnUpdate(ts);
            }

            // Clearing screen
            // uint32_t idx = m_CmdQueue.AcquireNextImage();
            // m_CmdQueue.WaitIdleFence();
            // m_CmdQueue.SubmitAsync(m_CmdBuffer[idx]);
            // // m_CmdQueue.WaitIdle();
            // m_CmdQueue.WaitIdleFence();
            // m_CmdQueue.Presentation(idx);
            Renderer::FlushScene();

            // m_RenderPass.BeginPass(idx, m_CmdBuffer[idx], VK_SUBPASS_CONTENTS_INLINE, m_ShaderPipeline.GetShaderPipeline());
            // vkCmdBindPipeline(m_CmdBuffer[idx], VK_PIPELINE_BIND_POINT_GRAPHICS, m_ShaderPipeline.GetShaderPipeline());
            // vkCmdDraw(m_CmdBuffer[idx], 3, 1, 0, 0);
            // m_RenderPass.EndPass(m_CmdBuffer[idx]);

            for(const auto& layer : m_Layers){
                layer->OnUIRender();
            }

            glfwPollEvents();
        }
    }

};