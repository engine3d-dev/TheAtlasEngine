#pragma once

#include <string_view>
#include <vulkan/vulkan_core.h>
#include <vector>

namespace engine3d{
    namespace vk{
        class VulkanFramebuffer{
        public:
            VulkanFramebuffer(const std::string_view debugName);

            // static void InitializeFramebuffer();

        private:
            std::vector<VkFramebuffer> m_Framebuffers;
        };
    };
};