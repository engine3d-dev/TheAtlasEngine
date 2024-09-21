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

	// static VkClearValue clearColorValue;
	
	// //! @note creating, recording, and submitting command buffers
	// static VkCommandPool g_ClearColorCmdPool;
	// static VkCommandBuffer g_ClearColorCommandBuffer;
	// static VkQueue g_MainSubmissionQueue;
    // static VkFence g_ClearColorFence;
    // // static uint32_t g_CurrentImageFrameIdx = 0;

    // //! @note Semaphores
    // static VkSemaphore g_PresentationCompletedSemaphore, g_RendererCompletedSemaphore;

    static vk::VulkanCommandBuffer g_CommandBuffer;
    static vk::VulkanCommandQueue g_CommandQueue;
    static vk::VulkanRenderPass g_CommandRenderPass;
    static VkClearValue g_ClearColorValue;

    void Renderer::Initialize(){
        g_CommandBuffer = vk::VulkanCommandBuffer(vk::VulkanSwapchain::GetImagesSize());
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
        VkImageSubresourceRange imgRange = {
            .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
            .baseMipLevel = 0,
            .levelCount = 1,
            .baseArrayLayer = 0,
            .layerCount = 1
        };

        /*
        void Begin(VkCommandBuffer commandBuffer, VkCommandBufferUsageFlags usageFlags){
            VkCommandBufferBeginInfo beginInfo{
                .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
                .pNext = nullptr,
                .flags = usageFlags,
                .pInheritanceInfo = nullptr
            };

            VkResult res = vkBeginCommandBuffer(commandBuffer, &beginInfo);
            if(res != VK_SUCCESS){
                ConsoleLogError("vkBeginCommandBuffer error message is ==> {}", VkResultToString(res));
            }
        }

        void End(VkCommandBuffer buffer){
            VkResult res = vkEndCommandBuffer(buffer);
            if(res != VK_SUCCESS){
                ConsoleLogError("vkEndCommandBuffer errored message is {}", VkResultToString(res));
            }
        }
        */

       //! @note Replace the following code below with:
       /*
            VkCommandBufferUsageFlagBits flags = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT; 
            vk::record(cmdBuffer, flags, [](){
                
            });
       */
      /*
        Format

        BeginCmdBuffer();
        BeginRenderPass();

        Actual rendering commands...

        EndRenderPass();
        EndCmdBuffer();

    */
        /*
        // OLD VERSION OF THE API
        for(int i = 0; i < g_CommandBuffer.GetCmdBufferSize(); i++){
            auto& cmdBuffer = g_CommandBuffer[i];
            g_CommandRenderPass.SetCurrentFramebuffer(i);

            vk::Begin(cmdBuffer, VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT);
            g_CommandRenderPass.Begin(cmdBuffer, VK_SUBPASS_CONTENTS_INLINE);

            g_CommandRenderPass.End(cmdBuffer);
            vk::End(cmdBuffer);
        }
        */

        // NEW API
        VkCommandBufferUsageFlags flags = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT;
        vk::record(g_CommandBuffer, flags, [&renderPass = g_CommandRenderPass](VkCommandBuffer& buffer, uint32_t currentCmdBufferFrameIdx){
            renderPass.SetCurrentFramebuffer(currentCmdBufferFrameIdx);
            renderPass.Begin(buffer, VK_SUBPASS_CONTENTS_INLINE);
            renderPass.End(buffer);
        });

        //! @note Recording another set of commands.
        // VkCommandBufferUsageFlags flags2 = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT;
        // vk::record(g_CommandBuffer, flags2, [&renderPass = g_CommandRenderPass](VkCommandBuffer& buffer, uint32_t currentCmdBufferFrameIdx){
        //     renderPass.SetCurrentFramebuffer(currentCmdBufferFrameIdx);
        //     renderPass.Begin(buffer, VK_SUBPASS_CONTENTS_INLINE);
        //     renderPass.End(buffer);
        // });
	}

    void Renderer::Presentation(){
        /**
        @note Thinking of a better way of handling presentation.
        @note So when we get to shaders, we can at least not worry about the miniscule steps that make our API more prone to bugs.
        
        */
        uint32_t idx = g_CommandQueue.AcquireNextImage();
        g_CommandQueue.WaitIdleFence();
        g_CommandQueue.SubmitAsync(g_CommandBuffer.GetActiveBuffer(idx));
        g_CommandQueue.Presentation(idx);
        g_CommandQueue.WaitIdleFence();
	}

    VkClearValue* Renderer::GetClearColor(){ return &g_ClearColorValue; }
};
