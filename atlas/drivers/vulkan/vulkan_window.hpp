#pragma once
#include <drivers/swapchain.hpp>
#include <GLFW/glfw3.h>
#include <core/window.hpp>
#include <vulkan/vulkan_core.h>

namespace atlas::vk{
    class vk_window : public window{
    public:
        vk_window(uint32_t p_width, uint32_t p_height, const std::string& p_tag);
        virtual ~vk_window(){}
    private:
        uint32_t read_width() const override;
        uint32_t read_height() const override;

    private:
        static void on_resize(GLFWwindow* p_window, int p_width, int p_height);
    private:
        ref<swapchain> current_swapchain() override { return m_swapchain; }
        GLFWwindow* native_window() const override;
    private:
        GLFWwindow* m_window;
        ref<swapchain> m_swapchain;
        VkSurfaceKHR m_surface;
    };
};