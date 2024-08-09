#pragma once
#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>
#include <string>

struct GLFWwindow;
namespace engine3d{

    namespace vk{
        /**
        * @name Vulkan.h
        * @note Uses for instantiating our vulkan API
        * @note These are properties that will be used throughout our vulkan's backend
        * @note Instantiating when we initiate the logical/physical devices.
        */
        class VulkanPipeline{
        public:
            static void InitializePipeline();
            void CleanupPipeline();
            static GLFWwindow* GetCurrentWindow();
            static VkInstance& GetVkInstance();
            static VkSurfaceKHR& GetVkSurface();
            static std::string& GetApplicationTitle();
            static uint32_t GetWidth();
            static uint32_t GetHeight();
        };
    };
};