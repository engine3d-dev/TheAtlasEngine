#pragma once
#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>
#include <string>
#include <vector>

struct GLFWwindow;
namespace engine3d{
    //! @note Properties for our current window application.
    struct WindowApplicationProperties{
        uint32_t width, height;
        std::string title;
    };

    /**
    * @name Vulkan.h
    * @note Uses for instantiating our vulkan API
    * @note These are properties that will be used throughout our vulkan's backend
    * @note Instantiating when we initiate the logical/physical devices.
    */
    struct VulkanProperties{
        VkInstance instance;
        VkDebugUtilsMessengerEXT debugMessenger;
        VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
        VkDevice logicalDevice;
        VkQueue graphicsQueue;
        VkQueue presentQueue;
        VkSurfaceKHR surface;
        VkSwapchainKHR swapChain;
        std::vector<VkImage> swapchainImages;
        VkFormat swapChainImageFormat;
        VkExtent2D swapChainExtent;
        WindowApplicationProperties windowProperties;
        GLFWwindow* glfwWindowInstance; // our current window instance.
    };

    class VulkanPipeline{
    public:
        //! @note Used for instantiating our Vulkan API
        void InitializeVulkan();

        //! @note Cleanup operation making sure vulkan is closing properly
        void CleanVulkanPipeline();

        //! @note Use is for knowing if our vulkan API has been enabled.
        //! @note Getting our properties for the vulkan pipeline.
        static VulkanProperties& GetVulkanProperties();
        static GLFWwindow* GetCurrentWindow();
        static bool EnableValidationLayer();
        static std::vector<const char*> ValidationLayers();
        static WindowApplicationProperties GetWindowData();
        static std::vector<const char*> DeviceExtensions();

    private:
        //! @note Setting up Vulkan Instnace
        void SetupVulkanInstance();

        void SetupDebugMessenger();
    };
};