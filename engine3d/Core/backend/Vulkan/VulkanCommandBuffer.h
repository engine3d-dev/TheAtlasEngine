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
            VulkanCommandBuffer(uint32_t count);
            ~VulkanCommandBuffer();

            /**
             * @param CommandBuffer is the buffer that we take as the handle
             * @param VkCommandUsageFlags represents the flags tell how the command buffers will be used through submission
            */
            void Begin(VkCommandBuffer commandBuffer, VkCommandBufferUsageFlags usageFlags);
            void End(VkCommandBuffer buffer);

            void RecordCommandBuffers();

            /** 
             * @note There are the process of submitting an image through the command buffer queue
             * @param AcquireImage - getting an index to free image N.
             * @param SubmitCommand - Submitting a command buffer to render to image N.
             * @param Presentation - Presenting image N
            */
            void SubmitCommandBuffers();

            VkCommandBuffer operator[](uint32_t idx);

        private:
            uint32_t m_ImagesCount = 0; // number of images
            std::vector<VkCommandBuffer> m_CommandBuffers;
            VkCommandPool m_CommandPool;
        };
    }; 
};