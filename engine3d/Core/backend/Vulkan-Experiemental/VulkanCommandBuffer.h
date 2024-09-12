#pragma once
#include <vulkan/vulkan.h>
#include <vector>

namespace engine3d{
    namespace vk{
        /**
         * @name Vulkan Command Buffer
         * @note Abstraction of our command buffer in Vulkan
         * @note Typically there are a few stages to the command buffer lifecylcle.
         * @param States Allocate (Initial) => Begin (Recording) => End (Execute) => Complete (Submission)
         * @note While chechking during recording if command buffer's invalid then it'll reset back to it's initial state.
         * @note If invalid just before submission then it would complete with a one-time submit(pending) then go through an invalid state before resetting to its initial state.
         * 
         * @param MemoryFragmentation
         * @note Since Vulkan allows for multiple command buffers where users can reset its initial state, be aware to onto cause memory fragmentation.
         * @note For memory fragmentation, Vulkan allows for the notion of VkCommandPool (Command Buffer Pools)
         * @note To think about this is an optimized memory allocator to reduce memory fragmentations tailored for these specific jobs.
         * @note Addition to VkCommandPool must externally and explicitly be synchronized if you want to allocate, record, or reset commands.
         * @note Better approach to this is to allocate a pool for each thread to allow safely recording different command buffers in different threads in parallel. (Which OpenGL does not allow you do to do)
        */
        class VulkanCommandBuffer{
        public:
            VulkanCommandBuffer() = default;
            /**
             * @param count is the size of command buffers this current command buffer structure contains.
            */
            VulkanCommandBuffer(uint32_t count);
            ~VulkanCommandBuffer();

            /**
             * @param begin indicates when the command buffer should start recording commands
             * @param end tells the command buffer where to stop recording commands.
            */
            void begin(VkCommandBufferUsageFlags flags);
            void end();

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