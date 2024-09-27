#include <backend/Vulkan/VulkanCommandQueue.h>
#include <backend/Vulkan-Experiemental/VulkanSwapchain.h>
#include <backend/Vulkan-Experiemental/VulkanCommandBuffer.h>
#include <backend/Vulkan-Experiemental/VulkanRenderPass.h>
#include <Core/Renderer/Renderer.h>
#include <Core/backend/Vulkan-Experiemental/Vulkan.h>
#include <Core/backend/Vulkan-Experiemental/VulkanDevice.h>
#include <Core/EngineLogger.h>
#include <Core/backend/utilities/helper_functions.h>
#include <limits>
#include <vulkan/vulkan_core.h>
#include <Core/backend/Vulkan-Experiemental/helper-functions.h>

namespace engine3d{
    struct RendererProperties{
		
    };

    static vk::VulkanCommandBuffer g_ClearCommandBuffer;
    static vk::VulkanCommandQueue g_CommandQueue;
    static vk::VulkanRenderPass g_CommandRenderPass;
    static VkClearValue g_ClearColorValue;

    void Renderer::Initialize(){
        VkCommandBufferUsageFlags flags = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT;
        g_ClearCommandBuffer = vk::VulkanCommandBuffer(flags, vk::VulkanSwapchain::GetImagesSize());
        // g_CommandBuffer = vk::VulkanCommandBuffer(2);
        g_CommandQueue = vk::VulkanCommandQueue(0);
        g_CommandRenderPass = vk::VulkanRenderPass("Clear Render Pass");
	}

	void Renderer::SetBackgroundColor(const std::array<float, 4> &rgba){
		for(size_t i = 0; i < rgba.size(); i++){
			g_ClearColorValue.color.float32[i] = rgba[i];
		}
	}

    void Renderer::RenderBasicClearColor(){
        // NEW API
        vk::record(g_ClearCommandBuffer, [&renderPass = g_CommandRenderPass](VkCommandBuffer& buffer, uint32_t currentCmdBufferFrameIdx){
            //! @note Proposal -- Wrapper API for at least how render passes are handled.
            /*
            

            */

            renderPass.SetCurrentFramebuffer(currentCmdBufferFrameIdx);
            renderPass.Begin(buffer, VK_SUBPASS_CONTENTS_INLINE);
            renderPass.End(buffer);
        });
	}

    void Renderer::Presentation(){
        /**
        @note Thinking of a better way of handling presentation.
        @note So when we get to shaders, we can at least not worry about the miniscule steps that make our API more prone to bugs.
        
        */
        uint32_t idx = g_CommandQueue.AcquireNextImage();
        g_CommandQueue.WaitIdleFence();
        g_CommandQueue.SubmitAsync(g_ClearCommandBuffer.GetActiveBuffer(idx));
        g_CommandQueue.Presentation(idx);
        g_CommandQueue.WaitIdleFence();
	}

    VkClearValue* Renderer::GetClearColor(){ return &g_ClearColorValue; }
};
