#pragma once
#include <cstdint>
#include <string>
#include <vulkan/vulkan_core.h>

class GLFWwindow;
namespace engine3d{
    namespace vk{
        class VulkanWindow{
        public:
            VulkanWindow(const uint32_t& p_Width, const uint32_t p_Height, const std::string& p_Title);

            std::string GetTitle() const;
            uint32_t GetWidth() const;
            uint32_t GetHeight() const;
            VkSwapchainKHR& CurrentWindowSwapchain() const;
        
            void Presentation();
            // void OnUpdateFrame();
        private:
            GLFWwindow* m_WindowHandler;
        };
    };
};


/*
Whiteboarding (When I resume back)


- Design how the windowing system may work.
- Look into how vulkan wrapper may handle multiple windowing API's
    - Such as SDL2, GLFW, and etc.
- API Overview (New Functions)
    - As<GLFWwindow>(m_Window);
        - Returns something like a GLFW window

    Another use-case
    - As<SDLwindow>(m_Window)
        - retrurns the SDL2 window that might have been implemented

- Look into the likeliness that there will be multiple monitor
    - One swapchain per monitor/display

*/