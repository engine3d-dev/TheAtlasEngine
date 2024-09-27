#pragma once
#include <vulkan/vulkan.h>
#include <vector>
#include <vulkan/vulkan_core.h>

namespace engine3d{
    namespace vk{
        /**
         * @param count is the size of command buffers this current command buffer structure contains.
         * 
         * @param p_CmdFlags used for specifying usage behavior for the current command buffer.
         * 
         * @param VkCommandBufferUsageFlags
         * 
         * @param VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT specifies each recording of cmd buffer will only be submitted once & buffer will reset/record again between each submission
         * 
         * @param VK_COMMAND_BUFFER_USAGE_RENDER_PASS_CONTINUE_BIT specifies a secondary command buffer is considered to be entirely inside a render pass.
         * If this is the primary command buffer, then this bit will be ignored.
         * 
         * @param VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT Specifies command buffer can be resubmitted to any queue of the same queue fam while in pending state and recording into multiple primary command buffers.
        */
        class VulkanCommandBuffer{
        public:
            VulkanCommandBuffer() = default;
            
            VulkanCommandBuffer(VkCommandBufferUsageFlags p_CmdFlags, uint32_t count);
            ~VulkanCommandBuffer();

            VkCommandBufferUsageFlags& GetUsageFlags();

            /** @note Returning our currently active command buffer **/
            VkCommandBuffer& GetActiveBuffer(uint32_t idx);

            /** @note getting size of command buffers that this wrapper for command buffers contain **/
            uint32_t Size() const;

        private:
            std::vector<VkCommandBuffer> m_CommandBuffers;
            VkCommandPool m_CommandPool;
        };
    };
};