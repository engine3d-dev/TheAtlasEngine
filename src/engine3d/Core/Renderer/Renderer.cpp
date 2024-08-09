#include <Core/EngineLogger.h>
#include <Core/backend/Vulkan/VulkanCommandBuffer.h>
#include <Core/backend/Vulkan/VulkanCommandQueue.h>
#include <Core/backend/Vulkan/Vulkan.h>
#include <Core/backend/Vulkan/VulkanDevice.h>
#include <Core/backend/Vulkan/VulkanSwapchain.h>
#include <Core/backend/Vulkan/VulkanRenderPass.h>
#include <Core/backend/Vulkan/Shaders/VulkanShaderModule.h>
#include <Core/backend/Vulkan/Shaders/VulkanShaderCompiler.h>
#include <Core/Renderer/Renderer.h>
#include <Core/backend/utilities/helper_functions.h>

namespace engine3d{
    struct RendererProperties{
        vk::VulkanCommandBuffer m_CommandBuffer;
        vk::VulkanCommandQueue m_CommandQueue;
        vk::VulkanRenderPass m_RenderPass;
        vk::VulkanShaderModule m_TriangleShaderVertModule;
        vk::VulkanShaderModule m_TriangleShaderFragModule;
        vk::VulkanShaderCompiler m_ShaderPipeline;
        VkClearColorValue color; // rgba color
        VkClearValue clearValue;
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
    }

    void Renderer::SetBackgroundColor(const std::array<float, 4>& rgba){
        g_properties.clearValue.color = {rgba[0], rgba[1], rgba[2], rgba[3]};
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
            // VkImageMemoryBarrier presentationToClearBarrier = {};
			// presentationToClearBarrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
			// presentationToClearBarrier.srcAccessMask = VK_ACCESS_MEMORY_READ_BIT;
			// presentationToClearBarrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
			// presentationToClearBarrier.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED;
			// presentationToClearBarrier.newLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
			// presentationToClearBarrier.srcQueueFamilyIndex = presentQueueFamily;
			// presentationToClearBarrier.dstQueueFamilyIndex = presentQueueFamily;
			// presentationToClearBarrier.image = vk::VulkanSwapchain::GetImage(i);
			// presentationToClearBarrier.subresourceRange = imgRange;

			// // Change layout of image to be optimal for presenting
			// VkImageMemoryBarrier clearToPresentBarrer = {};
			// clearToPresentBarrer.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
			// clearToPresentBarrer.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
			// clearToPresentBarrer.dstAccessMask = VK_ACCESS_MEMORY_READ_BIT;
			// clearToPresentBarrer.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
			// clearToPresentBarrer.newLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
			// clearToPresentBarrer.srcQueueFamilyIndex = presentQueueFamily;
			// clearToPresentBarrer.dstQueueFamilyIndex = presentQueueFamily;
			// clearToPresentBarrer.image = vk::VulkanSwapchain::GetImage(i);
			// clearToPresentBarrer.subresourceRange = imgRange;

            vk::Begin(g_properties.m_CommandBuffer[i], VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT);

            g_properties.m_RenderPass.GetBeginInfo().framebuffer = g_properties.m_RenderPass.GetFramebuffer(i);
            // g_properties.m_RenderPass.SetCurrentFramebuffer(i);
            // g_properties.m_RenderPass
            vkCmdBeginRenderPass(g_properties.m_CommandBuffer[i], &g_properties.m_RenderPass.GetBeginInfo(), VK_SUBPASS_CONTENTS_INLINE);
            // g_properties.m_RenderPass.Begin(g_properties.m_CommandBuffer[i], VK_SUBPASS_CONTENTS_INLINE);

            // vkCmdPipelineBarrier(g_properties.m_CommandBuffer[i], VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT, 
            // 0, // dependency flags
            // 0, nullptr, // memory barriers
            // 0, nullptr, // buffer memory barriers 
            // 1, &presentationToClearBarrier);

            // vkCmdClearColorImage(g_properties.m_CommandBuffer[i], vk::VulkanSwapchain::GetImage(i), VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, &g_properties.clearValue.color, 1, &imgRange);
            // vkCmdPipelineBarrier(g_properties.m_CommandBuffer[i], VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT, 0, 0, nullptr, 0, nullptr, 1, &clearToPresentBarrer);

            // g_properties.m_RenderPass.End(g_properties.m_CommandBuffer[i]);
            vkCmdEndRenderPass(g_properties.m_CommandBuffer[i]);
            vk::End(g_properties.m_CommandBuffer[i]);
        }
    }

    //! @note Typically when we submit everything (context of our scene) then we flush that scene into render pass just before we render and send that to the screen.
    void Renderer::FlushScene(){
        uint32_t idx = g_properties.m_CommandQueue.AcquireNextImage();
        g_properties.m_CommandQueue.WaitIdleFence();
        g_properties.m_CommandQueue.SubmitAsync(g_properties.m_CommandBuffer[idx]);
        g_properties.m_CommandQueue.WaitIdleFence();
        g_properties.m_CommandQueue.Presentation(idx);
    }

    VkClearValue* Renderer::GetClearColor(){
        return &g_properties.clearValue;
    }
};