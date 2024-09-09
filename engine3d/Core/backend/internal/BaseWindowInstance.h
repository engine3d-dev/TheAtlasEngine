#pragma once
#include <vulkan/vulkan.h>

namespace engine3d{
    /**
     * @name Window
     * @name Interface for enabling usres to customize for multi-monitors
     * 
     * @note There are a few API's that I want to go with this.
     * @note The API's should be flexible in a sense of allowing users to implement their own customizable monitors.
     * @note Implementing their own versions of a window instance
     * 
     * @note Some API's helper functions would be:
     * 
     * @note Being able to mirror to two monitors. If needed.
     * engine3d::mirror(m_Window1, m_Window2);
     * 
     * @note Split - meaning that we are writing to two swapchains
     * @note Keep in mind -- per window monitor there can only be one swapchain.
     * engine3d::split(m_Window1, m_Window2);
     * 
     * @note Before I forget --- Here are some TODO's for windowing system
     * @note Windows knows about the swapchain and framebuffer and images.
     * @note Having a generic enough API that works across windowing systems. That is also quite sensible.
    */
    class BaseWindowInstance{
    public:
        VkSwapchainKHR& GetCurrentSwapchain();
    };
};