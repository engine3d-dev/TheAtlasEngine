#pragma once
#include <functional>
#include <vector>
// #include <Core/backend/Vulkan/VulkanCommandBuffer.h>
#include <vulkan/vulkan_core.h>

namespace engine3d{
    namespace vk{
        /**
         * @name Vulkan Command Queue
         * @note This queue will be used to submit command buffers through
        */
        class VulkanCommandQueue{
        public:
            VulkanCommandQueue() = default;
            VulkanCommandQueue(uint32_t queueIdx);
            // void Submit(std::function<void()>& attribute);
            uint32_t AcquireNextImage();

            void SubmitSync(VkCommandBuffer buffer);
            void SubmitAsync(VkCommandBuffer buffer);

            void Presentation(uint32_t imgIdx);

            //! @note Should check if we're idle.
            //! @note Means to hang until command queue are done with our images stored
            void WaitIdle();
        private:
            VkQueue m_CmdQueue = VK_NULL_HANDLE;
            VkSemaphore m_RenderCompleteSemaphore; // signal when rendering complete
            VkSemaphore m_PresentCompleteSemaphore; // signal when presentation's completed
        };
    };
};