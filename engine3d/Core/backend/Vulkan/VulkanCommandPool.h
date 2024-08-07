#pragma once
#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>

namespace engine3d{
    namespace vk{
        /**
         * @name VkCommandPool abstraction
         * @note Contain opaque objects command buffers memory's allocated from.
         * @note Command pools externally synchronized, meaning cmd pools must not be used concurrently in multiple threads
         * @note This includes recording commands on any command buffer allocated from pool as well operations that allocate free, reset command buffers or pool itself.
         * @note Disclaimer - Still deciding if I need this VulkanCommandPool and seeing what other reasons of using this.
        */
        class VulkanCommandPool{
        public:
            void InitializeCommandPool();

            static VkCommandPool& GetVkCommandPoolInstance();


        private:
            
        };
    };
};