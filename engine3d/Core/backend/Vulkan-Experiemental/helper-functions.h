#pragma once
#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>
#include <backend/Vulkan-Experiemental/VulkanCommandBuffer.h>

namespace engine3d::vk{
    /**
     * @name begin
     * @param buffer is the command buffer to record too.
     * @param p_UsageFlags are for specifying how we want to record our commands
    */
    void begin(const VkCommandBuffer& buffer, VkCommandBufferUsageFlags p_UsageFlags);

    /**
     * @param buffer is the command buffer for stopping recording too.
    */
    void end(const VkCommandBuffer& buffer);

    /**
     * @name record
     * @note Collecting our command to send to the specified command buffer
    * @param commandBuffer is the buffer we want to record our commands to
    * @param VkCommandBufferUsageFlags is the flags to set when we start recording commands to our command buffer
    * @param UCommand will enable users to define what specifications of our commands we want to record.
    */
    template<typename UCommand>
    void record(VulkanCommandBuffer& commandBuffer, VkCommandBufferUsageFlags flags, const UCommand& p_CommandToCollect){
        for(uint32_t currCmdBufferIdx = 0; currCmdBufferIdx < commandBuffer.Size(); currCmdBufferIdx++){
            auto& buffer =  commandBuffer.GetActiveBuffer(currCmdBufferIdx);

            begin(buffer, flags);
            p_CommandToCollect(buffer, currCmdBufferIdx);
            end(buffer);
        }
    }
};