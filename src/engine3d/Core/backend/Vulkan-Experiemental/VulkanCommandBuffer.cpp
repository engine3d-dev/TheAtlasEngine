#include "Core/backend/Vulkan-Experiemental/VulkanSwapchain.h"
#include "Core/backend/utilities/helper_functions.h"
#include <Core/backend/Vulkan-Experiemental/VulkanCommandBuffer.h>
#include <Core/backend/Vulkan-Experiemental/VulkanDevice.h>
#include <Core/EngineLogger.h>
#include <vulkan/vulkan_core.h>

namespace engine3d::vk{
    /**
     * @note We create a single command pool for our command buffer
     * @note We create a single command buffer with the information on how they should do allocation tasks.
     * @note TODO -- command pools should be in a different implementation task as later on we may be dealing with mutliple command buffers
     * @note Where we may want to record these commands, reset them, or even allocate them in parallel
    */
    VulkanCommandBuffer::VulkanCommandBuffer(uint32_t commandBufferSizeCount) {
        //! @note Initializing Command Buffer Pool
        //! @note TODO --- Having this be a separate implementation since we will be dealing with multiple command buffers in the future.
        //! @note For now we will have command pool in here.
        // m_ImagesCount = imageSizeCount;
        VkCommandPoolCreateInfo commandPoolCreateInfo = {
            .sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
            .pNext = nullptr,
            .flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT,
            .queueFamilyIndex = VulkanDevice::GetLogicalDevice().QueueFamily(),
        };

        m_CommandBuffers.resize(commandBufferSizeCount);

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


    /**
        vk::record(commandBuffer, flags, [](){
              // do stuff...
        });


        static void record(VulkanCommandBuffer, flags, UCommand cmd){
            commandBuffer.begin(flags);
            cmd();
            commandBuffer.end();
        }

        @note Option #1 -- If there are multiple command buffers this is what it would look like:

        for(int i = 0; i < m_CommandBuffers.size(); i++){
            vk::record(m_CommandBuffers[i], flags, [](){

            });
        }

        @note Option #2
        @note Esentially this will record commands to this buffer - that also depends how many command buffers is stored here.
        vk::record(m_CommandBuffers, flags, [](){});

        @note Backend would look
        void record(m_CommandBuffers, flags, UCommand command){
            for(int i = 0; i < size; i++){
                auto& currCmdBuffer = m_CommandBuffers[i];
                currCmdBuffer.begin(flags);
                command();
                currCmdBuffer.end();
            }
        }

    */

    VkCommandBuffer& VulkanCommandBuffer::GetActiveBuffer(uint32_t idx) {
        return m_CommandBuffers[idx];
    }

    uint32_t VulkanCommandBuffer::Size() const{
        return m_CommandBuffers.size();
    }







    // void VulkanCommandBuffer::RecordClearBackgroundColor(float r, float g, float b, float a){
    //     VkClearColorValue clearColorValue = { r, g, b, a};

    //     VkImageSubresourceRange imgRange = {
    //         .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
    //         .baseMipLevel = 0,
    //         .levelCount = 1,
    //         .baseArrayLayer = 0,
    //         .layerCount = 1
    //     };

    //     uint32_t presentQueueFamily = VulkanDevice::GetLogicalDevice().QueueFamily();

    //     //! @param TODO -- Should fix the way we handle command buffers... am not liking this API!
    //     //! @note Used for recording the clear instructions before submission.
    //     for(uint32_t i = 0; i < m_CommandBuffers.size(); i++){
    //         //! @param VK_IMAGE_LAYOUT_GENERAL layouts specify the pixels packed together inside actual memory of the image.
    //         //! @note Use of different layouts are for performance, whereas your GPU may perform better utilizing different layouts at diff stages of pipeline.
    //         //! @param VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT is used to make the state of this recorded buffer as a simultaneously use bit
    //         // Begin(m_CommandBuffers[i], VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT);

    //         vkCmdClearColorImage(m_CommandBuffers[i], VulkanSwapchain::GetImage(i), VK_IMAGE_LAYOUT_GENERAL, &clearColorValue, 1, &imgRange);
    //         End(m_CommandBuffers[i]);
    //     }

    //     ConsoleLogInfo("VulkanCommandBuffer::Record() called!");
    // }
};