#pragma once
#include <vulkan/vulkan_core.h>

namespace engine3d{
    namespace vk{
        /**
         * @name Vulkan Command Queue
         * @note This queue will be used to submit command buffers through
        * @note There are the process of submitting an image through the command buffer queue
        * @param AcquireImage - getting an index to free image N.
        * @param SubmitCommand - Submitting a command buffer to render to image N.
        * @param Presentation - Presenting image N
        */
        class VulkanCommandQueue{
        public:
            VulkanCommandQueue() = default;
            VulkanCommandQueue(uint32_t queueIdx);
            // void Submit(std::function<void()>& attribute);
            uint32_t AcquireNextImage();

            void ResetCommandBufferToEnqueue();

            VkQueue& GetVkQueueInstance();
            
            /**
             * @note Difference between SubmitAsync and SubmitSync
             * @param SubmitAsync just means that we have sempahores waiting to send a signal to indicate that the rendering/pressentations finished
             * @param SubmitSync means that we do not have any waiting semaphores
            */
            void SubmitSync(VkCommandBuffer buffer);
            void SubmitAsync(VkCommandBuffer buffer);

            void Presentation(uint32_t imgIdx);

            //! @note Should check if we're idle.
            //! @note Means to hang until command queue are done with our images stored
            void WaitIdle();

            void WaitIdleFence();
        private:
            VkQueue m_CmdQueue = VK_NULL_HANDLE;
            VkSemaphore m_RenderCompleteSemaphore = VK_NULL_HANDLE; // signal when rendering complete
            VkSemaphore m_PresentCompleteSemaphore = VK_NULL_HANDLE; // signal when presentation's completed

            VkFence m_RenderCompleteFence = VK_NULL_HANDLE;
            VkFence m_PresentCompleteFence = VK_NULL_HANDLE;
            VkFence m_RenderFence;
        };
    };
};