#include <Core/backend/Vulkan-Experiemental/VulkanSwapchain.h>
#include <Core/backend/utilities/helper_functions.h>
#include <Core/backend/Vulkan-Experiemental/VulkanCommandBuffer.h>
#include <Core/backend/Vulkan-Experiemental/VulkanDevice.h>
#include <Core/EngineLogger.h>
#include <vulkan/vulkan_core.h>

namespace engine3d::vk{
    static VkCommandBufferUsageFlags g_UsageFlags;
    /**
     * @note We create a single command pool for our command buffer
     * @note We create a single command buffer with the information on how they should do allocation tasks.
     * @note TODO -- command pools should be in a different implementation task as later on we may be dealing with mutliple command buffers
     * @note Where we may want to record these commands, reset them, or even allocate them in parallel
    */
    VulkanCommandBuffer::VulkanCommandBuffer(VkCommandBufferUsageFlags p_CmdUsageFlags, uint32_t commandBufferSizeCount) {
        //! @note Initializing Command Buffer Pool
        //! @note TODO --- Having this be a separate implementation since we will be dealing with multiple command buffers in the future.
        //! @note For now we will have command pool in here.
        g_UsageFlags = p_CmdUsageFlags;
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

    VkCommandBufferUsageFlags& VulkanCommandBuffer::GetUsageFlags(){
        return g_UsageFlags;
    }

    VkCommandBuffer& VulkanCommandBuffer::GetActiveBuffer(uint32_t idx) {
        return m_CommandBuffers[idx];
    }

    uint32_t VulkanCommandBuffer::Size() const{
        return m_CommandBuffers.size();
    }

};