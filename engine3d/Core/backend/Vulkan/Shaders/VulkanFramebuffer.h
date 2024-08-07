#pragma once

#include <vulkan/vulkan_core.h>
namespace engine3d{
    namespace vk{
        /**
         * @name Vulkan Framebuffer
         * @note Abstraction of the vulkan's API way of initiating framebuffers
        */
        class VulkanFramebuffer{
        public:

        private:
            VkFramebuffer m_FramebufferInstance;
        };
    };
};