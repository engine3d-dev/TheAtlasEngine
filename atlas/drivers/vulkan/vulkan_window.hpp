#pragma once
#include <GLFW/glfw3.h>
#include <core/window.hpp>
#include <drivers/swapchain.hpp>
#include <vulkan/vulkan_core.h>

namespace atlas::vk {
    class vk_window : public window {
    public:
        vk_window(uint32_t p_width,
                  uint32_t p_height,
                  const std::string& p_tag);
        ~vk_window() override = default;

    private:
        [[nodiscard]] uint32_t read_width() const override;
        [[nodiscard]] uint32_t read_height() const override;

    private:
        static void on_resize();

    private:
        ref<swapchain> current_swapchain() override { return m_swapchain; }
        [[nodiscard]] GLFWwindow* native_window() const override;

    private:
        GLFWwindow* m_window;
        ref<swapchain> m_swapchain;
        VkSurfaceKHR m_surface;
    };
};