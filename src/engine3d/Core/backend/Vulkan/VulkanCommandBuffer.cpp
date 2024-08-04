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
            .flags = 0,
            .queueFamilyIndex = VulkanDevice::GetLogicalDevice().QueueFamily(),
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

        if(vkAllocateCommandBuffers(VulkanDevice::GetVkLogicalDeviceInstance(), &commandBufAllocInfo, m_CommandBuffers.data()) != VK_SUCCESS){
            CoreLogError("vkAllocateCommandBuffers error because was unsuccessful in VulkanCommandBuffer.cpp: VulkanCommandBuffer::VulkanCommandBuffer()!");
        }

        CoreLogInfo("VulkanCmdBuffer initiated!");
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
            CoreLogError("vkBeginCommandBuffer error message is ==> {}", VkResultToString(res));
        }
    }

    void VulkanCommandBuffer::End(VkCommandBuffer buffer){
        VkResult res = vkEndCommandBuffer(buffer);
        if(res != VK_SUCCESS){
            CoreLogError("vkEndCommandBuffer errored message is {}", VkResultToString(res));
        }
    }

    void VulkanCommandBuffer::RecordCommandBuffers(){
        //! @note Actual color that represents what our window will be cleared to.
        VkClearColorValue clearColorValue = { 1.0f, 0.f, 0.f, 0.f };

        VkImageSubresourceRange imgRange = {
            .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
            .baseMipLevel = 0,
            .levelCount = 1,
            .baseArrayLayer = 0,
            .layerCount = 1
        };

        //! @param TODO -- Should fix the way we handle command buffers... am not liking this API!
        //! @note Used for recording the clear instructions before submission.
        for(uint32_t i = 0; i < m_CommandBuffers.size(); i++){
            //! @param VK_IMAGE_LAYOUT_GENERAL layouts specify the pixels packed together inside actual memory of the image.
            //! @note Use of different layouts are for performance, whereas your GPU may perform better utilizing different layouts at diff stages of pipeline.
            //! @param VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT is used to make the state of this recorded buffer as a simultaneously use bit
            Begin(m_CommandBuffers[i], VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT);

            vkCmdClearColorImage(m_CommandBuffers[i], VulkanSwapchain::GetImage(i), VK_IMAGE_LAYOUT_GENERAL, &clearColorValue, 1, &imgRange);
            End(m_CommandBuffers[i]);
        }

        CoreLogInfo("VkCommandBuffer Recorded!");
    }

    void VulkanCommandBuffer::SubmitCommandBuffers(){
        
    }

    VkCommandBuffer VulkanCommandBuffer::operator[](uint32_t idx){
        return m_CommandBuffers[idx];
    }
};