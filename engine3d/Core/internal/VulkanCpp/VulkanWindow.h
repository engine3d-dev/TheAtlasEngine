#pragma once
#include "Window.h"
#include "internal/VulkanCpp/VulkanSwapchain.h"
#include <string>
#include <cstdint>
#include <vulkan/vulkan_core.h>

class GLFWwindow;
namespace engine3d{
    namespace vk{
        class VulkanWindow : public Window{
        public:
            VulkanWindow(uint32_t p_Width, uint32_t p_Height, const std::string& p_Title);

        protected:
            uint32_t Width() const override;
            uint32_t Height() const override;
            std::string Title() const override;
            
            bool CurrentWindowActive() const override;
            
            VkSurfaceKHR& VkSurface() override;

            //! @note Returns our current set native window API.
            GLFWwindow* NativeWindow() override;

            //! @note Update surface rendering every frame.
            void UpdatePerTick() override;

            void Presentation() override;

        private:
            GLFWwindow* m_Window;
            VkSurfaceKHR m_Surface;
            VulkanSwapchain* m_CurrentWindowSwapchain;
            bool m_IsCurrentWindowActive = false;
            uint32_t m_Width, m_Height;
            std::string m_Title;
        };
    };
};