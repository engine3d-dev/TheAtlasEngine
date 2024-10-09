#pragma once

#include <vulkan/vulkan_core.h>
namespace engine3d{
    namespace vk{
        /*
         * @name Vulkan
         * @note Used to instantiate the vulkan's application information.
         * @note Essentially setting Vulkan's global state that contains information about the application
         * @note Which includes extensions and validation layers used for debugging information.
         * @note VKInstance used to instantiate vulkan and allows the application to pass information about itself.
        */
        class Vulkan{
        public:
            static void InitializeVulkanCore();
            static void CleanupVulkanCore();

            static VkInstance& GetVkInstance();
        };
    };
};
