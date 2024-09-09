<<<<<<< HEAD
#include <backend/Vulkan/VulkanCommandQueue.h>
#include <backend/Vulkan-Experiemental/VulkanSwapchain.h>
#include <backend/Vulkan-Experiemental/VulkanCommandBuffer.h>
#include <backend/Vulkan-Experiemental/VulkanRenderPass.h>
#include <Core/Renderer/Renderer.h>
#include <Core/backend/Vulkan-Experiemental/Vulkan.h>
=======
#include "backend/Vulkan/VulkanCommandBuffer.h"
#include "backend/Vulkan/VulkanCommandQueue.h"
#include "backend/Vulkan/VulkanSwapchain.h"
#include <Core/Renderer/Renderer.h>
#include <Core/backend/Vulkan/Vulkan.h>
>>>>>>> fa59aaf07c9cbf80f5d25918c4d43b3cca25238f
#include <Core/backend/Vulkan-Experiemental/VulkanDevice.h>
#include <Core/EngineLogger.h>
#include <Core/backend/utilities/helper_functions.h>
#include <limits>
#include <vulkan/vulkan_core.h>
<<<<<<< HEAD
#include <Core/backend/Vulkan-Experiemental/helper-functions.h>
=======
>>>>>>> fa59aaf07c9cbf80f5d25918c4d43b3cca25238f

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
<<<<<<< HEAD
        // g_CommandBuffer = vk::VulkanCommandBuffer(2);
        g_CommandQueue = vk::VulkanCommandQueue(0);
        g_CommandRenderPass = vk::VulkanRenderPass("Clear Render Pass");
	}

	void Renderer::SetBackgroundColor(const std::array<float, 4> &rgba){
		for(size_t i = 0; i < rgba.size(); i++){
			g_ClearColorValue.color.float32[i] = rgba[i];
		}
	}

=======
        g_CommandQueue = vk::VulkanCommandQueue(0);
        g_CommandRenderPass = vk::VulkanRenderPass("Clear Render Pass");

        // vkGetDeviceQueue(vk::VulkanDevice::GetVkLogicalDeviceInstance(), vk::VulkanDevice::GetLogicalDevice().QueueFamily(), 0, &g_MainSubmissionQueue);

        // VkSemaphoreCreateInfo createInfo = {
        //     .sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO,
        //     .pNext = nullptr,
        //     .flags = 0
        // };

        // //! @note Instantiating our sempahore handler
        // VkResult presentationSemaphoreRes = vkCreateSemaphore(vk::VulkanDevice::GetVkLogicalDeviceInstance(), &createInfo, nullptr, &g_PresentationCompletedSemaphore);
        // VkResult rendererSemaphoreRes = vkCreateSemaphore(vk::VulkanDevice::GetVkLogicalDeviceInstance(), &createInfo, nullptr, &g_RendererCompletedSemaphore);

        // //! @note Then validating if the creation of the semaphore object was valid.
        // if(presentationSemaphoreRes != VK_SUCCESS){
        //     ConsoleLogError("vkCreateSemaphore error message is {}", vk::VkResultToString(presentationSemaphoreRes));
        // }

        // if(rendererSemaphoreRes != VK_SUCCESS){
        //     ConsoleLogError("vkCreateSemaphore error message is {}", vk::VkResultToString(rendererSemaphoreRes));
        // }

        // //! @note Initializing VkCommandBuffer
        // VkCommandPoolCreateInfo clearCmdPool = {
        //     .sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
        //     .pNext = nullptr,
        //     .flags = 0,
        //     .queueFamilyIndex = vk::VulkanDevice::GetLogicalDevice().QueueFamily()
        // };

        // VkResult cmdPoolResult = vkCreateCommandPool(vk::VulkanDevice::GetVkLogicalDeviceInstance(), &clearCmdPool, nullptr, &g_ClearColorCmdPool);

        // if(cmdPoolResult != VK_SUCCESS){
        //     ConsoleLogError("vkCreateCommandPool errored message!===>=\t\t{}", vk::VkResultToString(cmdPoolResult));
        // }

        // VkCommandBufferAllocateInfo cmdBufAllocInfo = {
        //     .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
        //     .pNext = nullptr,
        //     .commandPool = g_ClearColorCmdPool,
        //     .level = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
        //     .commandBufferCount = 1
        // };

        // VkResult cmdBufAllocResult = vkAllocateCommandBuffers(vk::VulkanDevice::GetVkLogicalDeviceInstance(), &cmdBufAllocInfo, &g_ClearColorCommandBuffer);

        // if(cmdBufAllocResult != VK_SUCCESS){
        //     ConsoleLogError("vkAllocateCommandBuffers error because was unsuccessful in VulkanCommandBuffer.cpp: VulkanCommandBuffer::VulkanCommandBuffer()!");
        // }


	}

	void Renderer::SetBackgroundColor(const std::array<float, 4> &rgba){
		/* clearColorValue.color.float32 = rgba.data(); */
		for(size_t i = 0; i < rgba.size(); i++){
			g_ClearColorValue.color.float32[i] = rgba[i];
		}
	}

>>>>>>> fa59aaf07c9cbf80f5d25918c4d43b3cca25238f
    void Renderer::RenderBasicClearColor(){
        VkImageSubresourceRange imgRange = {
            .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
            .baseMipLevel = 0,
            .levelCount = 1,
            .baseArrayLayer = 0,
            .layerCount = 1
        };

<<<<<<< HEAD
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

=======
        for(int i = 0; i < g_CommandBuffer.GetCmdBufferSize(); i++){
            auto& cmdBuffer = g_CommandBuffer[i];
            g_CommandRenderPass.SetCurrentFramebuffer(i);

            vk::Begin(cmdBuffer, VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT);
            g_CommandRenderPass.Begin(cmdBuffer, VK_SUBPASS_CONTENTS_INLINE);

            g_CommandRenderPass.End(cmdBuffer);
            vk::End(cmdBuffer);
        }
	}

    void Renderer::Presentation(){
        uint32_t idx = g_CommandQueue.AcquireNextImage();
        g_CommandQueue.WaitIdleFence();
        g_CommandQueue.SubmitAsync(g_CommandBuffer[idx]);
        g_CommandQueue.Presentation(idx);
        g_CommandQueue.WaitIdleFence();
	}

>>>>>>> fa59aaf07c9cbf80f5d25918c4d43b3cca25238f
    VkClearValue* Renderer::GetClearColor(){ return &g_ClearColorValue; }
};
