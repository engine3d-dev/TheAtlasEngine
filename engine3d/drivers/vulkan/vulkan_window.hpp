#pragma once
#include <drivers/swapchain.hpp>
#include <GLFW/glfw3.h>
#include <core/window.hpp>
#include <vulkan/vulkan_core.h>

namespace engine3d::vk{
    class VulkanWindow : public Window{
    public:
        VulkanWindow(uint32_t Width, uint32_t Height, const std::string& Tag);
        virtual ~VulkanWindow(){}
    private:
        uint32_t Width() const override;
        uint32_t Height() const override;

        uint32_t FocusedWidth() const override;
        uint32_t FocusedHeight() const override;
    private:
        static void OnWindowResize(GLFWwindow* Window, int Width, int Height);
    private:
        Ref<Swapchain> CurrentSwapchain() override { return m_Swapchain; }
        GLFWwindow* NativeWindow() const override;
    private:
        GLFWwindow* m_WindowHandler;
        Ref<Swapchain> m_Swapchain;
        VkSurfaceKHR m_Surface;
    };
};