#include "Core/Event/InputPoll.h"
#include <Core/EngineLogger.h>
#include <Core/backend/Vulkan/VulkanCommandBuffer.h>
#include <Core/backend/Vulkan/VulkanCommandQueue.h>
#include <Core/backend/Vulkan/Vulkan.h>
#include <Core/backend/Vulkan/VulkanDevice.h>
#include <Core/backend/Vulkan/VulkanSwapchain.h>
#include <Core/backend/Vulkan/VulkanRenderPass.h>
#include <Core/backend/Vulkan/Shaders/VulkanShaderModule.h>
#include <Core/backend/Vulkan/Shaders/VulkanShaderPipelineBuilder.h>
#include <Core/Renderer/Renderer.h>
#include <Core/backend/utilities/helper_functions.h>
#include <Jolt/Jolt.h>
#include <vulkan/vulkan_core.h>

namespace engine3d{
    struct RendererProperties{
        vk::VulkanCommandBuffer m_CommandBuffer;
        vk::VulkanCommandQueue m_CommandQueue;
        vk::VulkanRenderPass m_RenderPass;
        vk::VulkanShaderModule m_TriangleShaderVertModule;
        vk::VulkanShaderModule m_TriangleShaderFragModule;
        vk::VulkanShaderPipelineBuilder m_ShaderPipeline;

        vk::VulkanShaderModule m_RedTriangleShaderVertModule;
        vk::VulkanShaderModule m_RedTriangleShaderFragModule;
        vk::VulkanShaderPipelineBuilder m_RedShaderPipeline;
        VkClearColorValue color; // rgba color
        VkClearValue clearValue;

        int m_SelectedShaderIdx = 0;
    };

    static RendererProperties g_properties;

    void Renderer::Initialize(){
        ConsoleLogInfo("Renderer::Initialize() created!");

        //! @note Initializes core vulkan API
        vk::VulkanPipeline::InitializePipeline(); // initializes our vulakn API
        vk::VulkanDevice::InitializeDevice(); // initializes our physical/logical devices
        vk::VulkanSwapchain::InitializeSwaphchain(); // initializes our swapchain and our VkImage/VkImageView's

        g_properties.m_CommandBuffer = vk::VulkanCommandBuffer(vk::VulkanSwapchain::GetImagesSize());

        //! @param index, zero - indicates getting first data in queue at index 0 (zero)
        //! @note TODO -- for cmd buffers/queues, shader modules, and render passes. Figuring out what parameters are important that users should specify when creating this instance
        //! @note This is because as it stands the API requires for instancing the object before initializing it, but should just utilize the constructor to cleanup the API for initiating our object's lifetimes.
        g_properties.m_CommandQueue = vk::VulkanCommandQueue(0);
        g_properties.m_RenderPass = vk::VulkanRenderPass("First render pass");
        // g_properties.m_RenderPass.InitializeRenderPass();
        // g_properties.m_RenderPass.InitializeFramebuffers();
        g_properties.m_TriangleShaderVertModule = vk::VulkanShaderModule("Resources/shaders/TriangleShader/triangle.vert.spirv");
        g_properties.m_TriangleShaderFragModule = vk::VulkanShaderModule("Resources/shaders/TriangleShader/triangle.frag.spirv");

        g_properties.m_ShaderPipeline = vk::VulkanShaderPipelineBuilder(g_properties.m_TriangleShaderVertModule.GetVkShaderModuleInstance(), g_properties.m_TriangleShaderFragModule.GetVkShaderModuleInstance(), g_properties.m_RenderPass.GetVkRenderPass());

        g_properties.m_RedTriangleShaderVertModule = vk::VulkanShaderModule("Resources/shaders/TriangleShader/RedTriangle.vert.spirv");
        g_properties.m_RedTriangleShaderFragModule = vk::VulkanShaderModule("Resources/shaders/TriangleShader/RedTriangle.frag.spirv");
        g_properties.m_RedShaderPipeline = vk::VulkanShaderPipelineBuilder(g_properties.m_RedTriangleShaderVertModule.GetVkShaderModuleInstance(), g_properties.m_RedTriangleShaderFragModule.GetVkShaderModuleInstance(), g_properties.m_RenderPass.GetVkRenderPass());


    }

    void Renderer::SetBackgroundColor(const std::array<float, 4>& rgba){
        g_properties.clearValue.color = {rgba[0], rgba[1], rgba[2], rgba[3]};
    }

    //! @note Typically when we submit everything (context of our scene) then we flush that scene into render pass just before we render and send that to the screen.
    void Renderer::FlushScene(){
        // g_properties.m_CommandBuffer.RecordClearBackgroundColor(rgba[0], rgba[1], rgba[2], rgba[3]);

        // VkClearColorValue clearColorValue = { 0.0f, 0.5f, 0.5f, 0.f};

        // Clearing screen
        //! @note TODO -- may use the function call that already does this. Should improve the renderer API so we can do primitives and involve shaders as well.
        //! @note Probably will use the function RecordClearBackgroundColor(...) but doing it manually to see how I may want to
        VkImageSubresourceRange imgRange = {
            .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
            .baseMipLevel = 0,
            .levelCount = 1,
            .baseArrayLayer = 0,
            .layerCount = 1
        };

        uint32_t presentQueueFamily = vk::VulkanDevice::GetLogicalDevice().QueueFamily();
        
        //! @note Command buffers for our images
        for(int i = 0; i < g_properties.m_CommandBuffer.GetCmdBufferSize(); i++){
            auto& cmdBuffer = g_properties.m_CommandBuffer[i];

            g_properties.m_RenderPass.SetCurrentFramebuffer(i);

            vk::Begin(cmdBuffer, VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT);
            g_properties.m_RenderPass.Begin(cmdBuffer, VK_SUBPASS_CONTENTS_INLINE);
            // VkPipeline pipeline = g_properties.m_ShaderPipeline.GetVkPipeline();

            if(InputPoll::IsKeyPressed(ENGINE_KEY_W)){
                vkCmdBindPipeline(cmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, g_properties.m_RedShaderPipeline.GetVkPipeline());
                // vkCmdDraw(cmdBuffer, 3, 1, 0, 0);
            }
            else{
                vkCmdBindPipeline(cmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, g_properties.m_ShaderPipeline.GetVkPipeline());
                // vkCmdDraw(cmdBuffer, 3, 1, 0, 0);
            }

            // vkCmdBindPipeline(cmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, g_properties.m_RedShaderPipeline.GetVkPipeline());
            // vkCmdBindPipeline(cmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, g_properties.m_ShaderPipeline.GetVkPipeline());
            // vkCmdBindPipeline(cmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline);
            vkCmdDraw(cmdBuffer, 3, 1, 0, 0);

            g_properties.m_RenderPass.End(cmdBuffer);
            vk::End(cmdBuffer);
        }

        uint32_t idx = g_properties.m_CommandQueue.AcquireNextImage();
        g_properties.m_CommandQueue.WaitIdleFence();
        g_properties.m_CommandQueue.SubmitAsync(g_properties.m_CommandBuffer[idx]);
        g_properties.m_CommandQueue.Presentation(idx);
        g_properties.m_CommandQueue.WaitIdleFence();
    }

    VkClearValue* Renderer::GetClearColor(){
        return &g_properties.clearValue;
    }
};