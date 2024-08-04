#include "Core/backend/utilities/helper_functions.h"
#include <Core/EngineLogger.h>
#include <Core/backend/Vulkan/VulkanCommandQueue.h>
#include <Core/backend/Vulkan/VulkanDevice.h>
#include <Core/backend/Vulkan/VulkanSwapchain.h>
#include <limits>

namespace engine3d::vk{

    //! @note Helper functions for creating the semaphores
    //! @note TODO --- Should change the way this works (?)
    static VkSemaphore make_semaphore(VkDevice device){
        //! @note For semaphores we need specify what attributes the sempahores will have enabled
        VkSemaphoreCreateInfo createInfo = {
            .sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO,
            .pNext = nullptr,
            .flags = 0
        };

        //! @note Instantiating our sempahore handler
        VkSemaphore semaphore;
        VkResult res = vkCreateSemaphore(device, &createInfo, nullptr, &semaphore);

        //! @note Then validating if the creation of the semaphore object was valid.
        if(res != VK_SUCCESS){
            CoreLogError("vkCreateSemaphore error message is {}", VkResultToString(res));
        }

        return semaphore;
    }

    VulkanCommandQueue::VulkanCommandQueue(uint32_t queueIdx){
        //! @note Fetching the queue's handler
        vkGetDeviceQueue(VulkanDevice::GetVkLogicalDeviceInstance(), VulkanDevice::GetLogicalDevice().QueueFamily(), queueIdx, &m_CmdQueue);

        CoreLogInfo("Queue Initiated!");

        //! @note Creating semaphores
        m_RenderCompleteSemaphore = make_semaphore(VulkanDevice::GetVkLogicalDeviceInstance());
        m_PresentCompleteSemaphore = make_semaphore(VulkanDevice::GetVkLogicalDeviceInstance());

    }

    //! @note This function returns the index of our image. Getting our next image.
    uint32_t VulkanCommandQueue::AcquireNextImage(){
        uint32_t imageIdx = 0;
        VkResult res = vkAcquireNextImageKHR(VulkanDevice::GetVkLogicalDeviceInstance(), VulkanSwapchain::GetVkSwapchainInstance(), std::numeric_limits<uint32_t>::max(), m_PresentCompleteSemaphore, nullptr, &imageIdx);

        if(res != VK_SUCCESS){
            CoreLogError("vkAcquireNextImageKHR error message {}", VkResultToString(res));
        }

        return imageIdx;
    }

    void VulkanCommandQueue::SubmitAsync(VkCommandBuffer buffer){

        //! @note Setting up our pipeline flags
        //! @note Setting up our submission flag info.
        //! @param VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT means to wait for clearing the window
        VkPipelineStageFlags waitFlags = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;

        VkSubmitInfo submissionInfo = {
            .sType = VK_STRUCTURE_TYPE_SUBMIT_INFO,
            .pNext = nullptr,
            .waitSemaphoreCount = 1,
            .pWaitSemaphores = &m_PresentCompleteSemaphore,
            .pWaitDstStageMask = &waitFlags,
            .commandBufferCount = 1,
            .pCommandBuffers = &buffer,
            .signalSemaphoreCount = 1,
            .pSignalSemaphores = &m_RenderCompleteSemaphore
        };

        VkResult res = vkQueueSubmit(m_CmdQueue, 1, &submissionInfo, nullptr);

        if(res != VK_SUCCESS){
            CoreLogError("vkQueueSubmit submission ERROR message = {}", VkResultToString(res));
        }
    }

    void VulkanCommandQueue::SubmitSync(VkCommandBuffer buffer){
        VkPipelineStageFlags waitFlags = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;

        VkSubmitInfo submissionInfo = {
            .sType = VK_STRUCTURE_TYPE_SUBMIT_INFO,
            .pNext = nullptr,
            .waitSemaphoreCount = 1,
            .pWaitSemaphores = VK_NULL_HANDLE,
            .pWaitDstStageMask = VK_NULL_HANDLE,
            .commandBufferCount = 1,
            .pCommandBuffers = &buffer,
            .signalSemaphoreCount = 0,
            .pSignalSemaphores = VK_NULL_HANDLE
        };

        VkResult res = vkQueueSubmit(m_CmdQueue, 1, &submissionInfo, nullptr);

        if(res != VK_SUCCESS){
            CoreLogError("vkQueueSubmit submission ERROR message = {}", VkResultToString(res));
        }
    }

    //! @note function to present our data to the display
    void VulkanCommandQueue::Presentation(uint32_t imgIdx){
        VkPresentInfoKHR presentInfo = {
            .sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR,
            .pNext = nullptr,
            .waitSemaphoreCount = 1, // (1) waiting for semaphores to finish before starting presenting the next image
            .pWaitSemaphores = &m_RenderCompleteSemaphore, // (2) waiting for semaphores to finish before starting presenting the next image
            .swapchainCount = 1, // number of swapchains
            .pSwapchains = &VulkanSwapchain::GetVkSwapchainInstance(),
            .pImageIndices = &imgIdx // number of image that we want to present
        };

        VkResult res = vkQueuePresentKHR(m_CmdQueue, &presentInfo);

        if(res != VK_SUCCESS){
            CoreLogError("vkQueuePresentKHR errored message = {}", VkResultToString(res));
        }
    }

    void VulkanCommandQueue::WaitIdle(){
        vkQueueWaitIdle(m_CmdQueue);
    }

};