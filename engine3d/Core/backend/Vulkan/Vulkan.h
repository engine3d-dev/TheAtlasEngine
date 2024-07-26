#pragma once
#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>
#include <string>

struct GLFWwindow;
namespace engine3d{
    //! @note Properties for our current window application.
    struct WindowApplicationProperties{
        uint32_t width, height;
        std::string title;
    };
    
    struct VulkanProperties{
        VkSurfaceKHR surface = VK_NULL_HANDLE;
        VkInstance instance = VK_NULL_HANDLE;
        VkDebugUtilsMessengerEXT debugMessenger = VK_NULL_HANDLE;
        WindowApplicationProperties windowProperties;
        GLFWwindow* glfwWindowInstance;
    };

    namespace vk{
        /**
        * @name Vulkan.h
        * @note Uses for instantiating our vulkan API
        * @note These are properties that will be used throughout our vulkan's backend
        * @note Instantiating when we initiate the logical/physical devices.
        */
        class VulkanPipeline{
        public:
            // ~VulkanPipeline();
            void InitializePipeline();
            void CleanupPipeline();
            static GLFWwindow* GetCurrentWindow();
            static VulkanProperties& GetVulkanProperties();

        private:
            //! @note Debug callback requires validatoin layers and debug utils to be enabled in extensions as it is not part of the vulkan core.
            void CreateDebugMessenger();
            void CreateSurface();

        };
    };
};