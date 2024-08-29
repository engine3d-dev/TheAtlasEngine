#include "Core/backend/Vulkan/VulkanSwapchain.h"
#include "Core/backend/utilities/helper_functions.h"
#include <Core/backend/Vulkan/VulkanCommandBuffer.h>
#include <Core/backend/Vulkan/VulkanDevice.h>
#include <Core/EngineLogger.h>
#include <vulkan/vulkan_core.h>

namespace engine3d::vk{
    /**
     * @note We create a single command pool for our command buffer
     * @note We create a single command buffer with the information on how they should do allocation tasks.
     * @note TODO -- command pools should be in a different implementation task as later on we may be dealing with mutliple command buffers
     * @note Where we may want to record these commands, reset them, or even allocate them in parallel
    */
    VulkanCommandBuffer::VulkanCommandBuffer(uint32_t imageSizeCount) {
        //! @note Initializing Command Buffer Pool
        //! @note TODO --- Having this be a separate implementation since we will be dealing with multiple command buffers in the future.
        //! @note For now we will have command pool in here.
        m_ImagesCount = imageSizeCount;
        VkCommandPoolCreateInfo commandPoolCreateInfo = {
            .sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
            .pNext = nullptr,
            .flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT, // this flag indicates to expect this individual command buffer made from this pool
            .queueFamilyIndex = VulkanDevice::GetLogicalDevice().QueueFamilyVkCount(),
        };

        m_CommandBuffers.resize(m_ImagesCount);

        VkResult res = vkCreateCommandPool(VulkanDevice::GetVkLogicalDeviceInstance(), &commandPoolCreateInfo, nullptr, &m_CommandPool);

        //! @note Creating our command buffers and initiating the command buffer information for allocation
        VkCommandBufferAllocateInfo commandBufAllocInfo = {
            .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
            .pNext = nullptr,
            .commandPool = m_CommandPool,
            .level = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
            .commandBufferCount = (uint32_t)m_CommandBuffers.size()
        };
        
        VkResult allocateResult = vkAllocateCommandBuffers(VulkanDevice::GetVkLogicalDeviceInstance(), &commandBufAllocInfo, m_CommandBuffers.data());
        if(res != VK_SUCCESS){
            ConsoleLogError("vkAllocateCommandBuffers error because was unsuccessful in VulkanCommandBuffer.cpp: VulkanCommandBuffer::VulkanCommandBuffer()!");
            ConsoleLogError("Errored message is\t\t{}", VkResultToString(allocateResult));
        }

        ConsoleLogInfo("VulkanCmdBuffer initiated!");
    }

    VulkanCommandBuffer::~VulkanCommandBuffer(){
    }

    void VulkanCommandBuffer::Begin(VkCommandBuffer commandBuffer, VkCommandBufferUsageFlags usageFlags){
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

    void VulkanCommandBuffer::End(VkCommandBuffer buffer){
        VkResult res = vkEndCommandBuffer(buffer);
        if(res != VK_SUCCESS){
            ConsoleLogError("vkEndCommandBuffer errored message is {}", VkResultToString(res));
        }
    }

    void VulkanCommandBuffer::RecordClearBackgroundColor(float r, float g, float b, float a){
        VkClearColorValue clearColorValue = { r, g, b, a};

        VkImageSubresourceRange imgRange = {
            .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
            .baseMipLevel = 0,
            .levelCount = 1,
            .baseArrayLayer = 0,
            .layerCount = 1
        };

        uint32_t presentQueueFamily = VulkanDevice::GetLogicalDevice().QueueFamilyVkCount();
        
        //! @note Command buffers for our images
        for(int i = 0; i < m_CommandBuffers.size(); i++){
            VkImageMemoryBarrier presentationToClearBarrier = {};
			presentationToClearBarrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
			presentationToClearBarrier.srcAccessMask = VK_ACCESS_MEMORY_READ_BIT;
			presentationToClearBarrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
			presentationToClearBarrier.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED;
			presentationToClearBarrier.newLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
			presentationToClearBarrier.srcQueueFamilyIndex = presentQueueFamily;
			presentationToClearBarrier.dstQueueFamilyIndex = presentQueueFamily;
			presentationToClearBarrier.image = VulkanSwapchain::GetImage(i);
			presentationToClearBarrier.subresourceRange = imgRange;

			// Change layout of image to be optimal for presenting
			VkImageMemoryBarrier clearToPresentBarrer = {};
			clearToPresentBarrer.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
			clearToPresentBarrer.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
			clearToPresentBarrer.dstAccessMask = VK_ACCESS_MEMORY_READ_BIT;
			clearToPresentBarrer.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
			clearToPresentBarrer.newLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
			clearToPresentBarrer.srcQueueFamilyIndex = presentQueueFamily;
			clearToPresentBarrer.dstQueueFamilyIndex = presentQueueFamily;
			clearToPresentBarrer.image = VulkanSwapchain::GetImage(i);
			clearToPresentBarrer.subresourceRange = imgRange;

            Begin(m_CommandBuffers[i], VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT);

            vkCmdPipelineBarrier(m_CommandBuffers[i], VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT, 
            0, // dependency flags
            0, nullptr, // memory barriers
            0, nullptr, // buffer memory barriers 
            1, &presentationToClearBarrier);

            vkCmdClearColorImage(m_CommandBuffers[i], VulkanSwapchain::GetImage(i), VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, &clearColorValue, 1, &imgRange);
            vkCmdPipelineBarrier(m_CommandBuffers[i], VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT, 0, 0, nullptr, 0, nullptr, 1, &clearToPresentBarrer);

            End(m_CommandBuffers[i]);
        }

        //! @param TODO -- Should fix the way we handle command buffers... am not liking this API!
        //! @note Used for recording the clear instructions before submission.
        // for(uint32_t i = 0; i < m_CommandBuffers.size(); i++){
        //     //! @param VK_IMAGE_LAYOUT_GENERAL layouts specify the pixels packed together inside actual memory of the image.
        //     //! @note Use of different layouts are for performance, whereas your GPU may perform better utilizing different layouts at diff stages of pipeline.
        //     //! @param VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT is used to make the state of this recorded buffer as a simultaneously use bit
        //     Begin(m_CommandBuffers[i], VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT);

        //     vkCmdClearColorImage(m_CommandBuffers[i], VulkanSwapchain::GetImage(i), VK_IMAGE_LAYOUT_GENERAL, &clearColorValue, 1, &imgRange);
        //     End(m_CommandBuffers[i]);
        // }

        ConsoleLogInfo("VulkanCommandBuffer::Record() called!");
    }

    void VulkanCommandBuffer::RecordCommandBuffers(){
        //! @note Actual color that represents what our window will be cleared to.
        VkClearColorValue clearColorValue = { 0.0f, 0.5f, 0.5f, 0.f };

        VkImageSubresourceRange imgRange = {
            .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
            .baseMipLevel = 0,
            .levelCount = 1,
            .baseArrayLayer = 0,
            .layerCount = 1
        };

        uint32_t presentQueueFamily = VulkanDevice::GetLogicalDevice().QueueFamilyVkCount();
        
        //! @note Command buffers for our images
        for(int i = 0; i < m_CommandBuffers.size(); i++){
            //! @note Why does this not work when we specify VK_QUEUE_FAMILY_IGNORED!
            // VkImageMemoryBarrier presentationToClearBarrier = {
            //     .sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
            //     .pNext = nullptr,
            //     .srcAccessMask = VK_ACCESS_MEMORY_READ_BIT,
            //     .dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT,
            //     .oldLayout = VK_IMAGE_LAYOUT_UNDEFINED,
            //     .newLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
            //     .srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
            //     .dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
            //     .image = VulkanSwapchain::GetImage(i),
            //     .subresourceRange = imgRange
            // };

            // //! @note Change layout of image to be optimal WHEN presenting
            // VkImageMemoryBarrier clearToPresentBarrer = {
            //     .sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
            //     .pNext = nullptr,
            //     .srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT,
            //     .dstAccessMask = VK_ACCESS_MEMORY_READ_BIT,
            //     .newLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR,
            //     .srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
            //     .dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
            //     .image = VulkanSwapchain::GetImage(i),
            //     .subresourceRange = imgRange
            // };

            VkImageMemoryBarrier presentationToClearBarrier = {};
			presentationToClearBarrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
			presentationToClearBarrier.srcAccessMask = VK_ACCESS_MEMORY_READ_BIT;
			presentationToClearBarrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
			presentationToClearBarrier.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED;
			presentationToClearBarrier.newLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
			presentationToClearBarrier.srcQueueFamilyIndex = presentQueueFamily;
			presentationToClearBarrier.dstQueueFamilyIndex = presentQueueFamily;
			presentationToClearBarrier.image = VulkanSwapchain::GetImage(i);
			presentationToClearBarrier.subresourceRange = imgRange;

			// Change layout of image to be optimal for presenting
			VkImageMemoryBarrier clearToPresentBarrer = {};
			clearToPresentBarrer.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
			clearToPresentBarrer.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
			clearToPresentBarrer.dstAccessMask = VK_ACCESS_MEMORY_READ_BIT;
			clearToPresentBarrer.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
			clearToPresentBarrer.newLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
			clearToPresentBarrer.srcQueueFamilyIndex = presentQueueFamily;
			clearToPresentBarrer.dstQueueFamilyIndex = presentQueueFamily;
			clearToPresentBarrer.image = VulkanSwapchain::GetImage(i);
			clearToPresentBarrer.subresourceRange = imgRange;

            Begin(m_CommandBuffers[i], VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT);

            vkCmdPipelineBarrier(m_CommandBuffers[i], VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT, 
            0, // dependency flags
            0, nullptr, // memory barriers
            0, nullptr, // buffer memory barriers 
            1, &presentationToClearBarrier);

            vkCmdClearColorImage(m_CommandBuffers[i], VulkanSwapchain::GetImage(i), VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, &clearColorValue, 1, &imgRange);
            vkCmdPipelineBarrier(m_CommandBuffers[i], VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT, 0, 0, nullptr, 0, nullptr, 1, &clearToPresentBarrer);

            End(m_CommandBuffers[i]);
        }

        //! @param TODO -- Should fix the way we handle command buffers... am not liking this API!
        //! @note Used for recording the clear instructions before submission.
        // for(uint32_t i = 0; i < m_CommandBuffers.size(); i++){
        //     //! @param VK_IMAGE_LAYOUT_GENERAL layouts specify the pixels packed together inside actual memory of the image.
        //     //! @note Use of different layouts are for performance, whereas your GPU may perform better utilizing different layouts at diff stages of pipeline.
        //     //! @param VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT is used to make the state of this recorded buffer as a simultaneously use bit
        //     Begin(m_CommandBuffers[i], VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT);

        //     vkCmdClearColorImage(m_CommandBuffers[i], VulkanSwapchain::GetImage(i), VK_IMAGE_LAYOUT_GENERAL, &clearColorValue, 1, &imgRange);
        //     End(m_CommandBuffers[i]);
        // }

        ConsoleLogInfo("VkCommandBuffer Recorded!");
    }

    VkCommandBuffer& VulkanCommandBuffer::operator[](uint32_t idx){
        return m_CommandBuffers[idx];
    }

    uint32_t VulkanCommandBuffer::GetCmdBufferSize(){
        return m_CommandBuffers.size();
    }
};