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
<<<<<<< HEAD
            /**
             * @param count is the size of command buffers this current command buffer structure contains.
            */
=======
>>>>>>> fa59aaf07c9cbf80f5d25918c4d43b3cca25238f
            VulkanCommandBuffer(uint32_t count);
            ~VulkanCommandBuffer();

            /**
<<<<<<< HEAD
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
=======
             * @param CommandBuffer is the buffer that we take as the handle
             * @param VkCommandUsageFlags represents the flags tell how the command buffers will be used through submission
            */
            void Begin(VkCommandBuffer commandBuffer, VkCommandBufferUsageFlags usageFlags);
            void End(VkCommandBuffer buffer);

            void RecordClearBackgroundColor(float r, float g, float b, float a = 1.0f);
            void RecordCommandBuffers();

            VkCommandBuffer& operator[](uint32_t idx);

            uint32_t GetCmdBufferSize();

        private:
            uint32_t m_ImagesCount = 0; // number of images
>>>>>>> fa59aaf07c9cbf80f5d25918c4d43b3cca25238f
            std::vector<VkCommandBuffer> m_CommandBuffers;
            VkCommandPool m_CommandPool;
        };
    };
};