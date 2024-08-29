#pragma once
#include <string>

namespace engine3d{
    /**
     * @name VulkanWindow
     * @note Defines our window that utilizes the Vulkan API
     * @note TODO -- Moving swapchain, and framebuffers into this windows definition
     * @note TODO -- Using VulkanWindow to handle swapchains and the presesntation step directly to Vulkan.
    */
    class VulkanWindow{
    public:
        VulkanWindow() = delete;
        
        //! @note Instantiates our vulkan window
        static VulkanWindow* Create();
        static std::string GetWindowTitle();
        static uint32_t GetWidth();
        static uint32_t GetHeight();
        // static void Maximize()
    };
};