#pragma once

#include <vulkan/vulkan_core.h>
namespace engine3d{
    namespace vk{
        class Vulkan{
        public:
            static void InitializeVulkanCore();
            static void CleanupVulkanCore();

            static VkInstance& GetVkInstance();
        };
    };
};