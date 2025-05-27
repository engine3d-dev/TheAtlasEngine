#pragma once
#include <core/window.hpp>
#include <GLFW/glfw3.h>
#include <drivers/vulkan-cpp/vk_swapchain.hpp>

namespace atlas::vk {
    class vk_window : public window {
    public:
        vk_window(const window_settings& p_settings);


    private:
        void center_window();

    private:
        [[nodiscard]] window_settings settings() const override;
        ref<swapchain> window_current_swapchain() override;
        [[nodiscard]] uint32_t read_acquired_next_frame() override;
        [[nodiscard]] GLFWwindow* native_window() const override;

    private:
        VkInstance m_instance_handler=nullptr;
        GLFWwindow* m_window_handler=nullptr;
        VkSurfaceKHR m_window_surface=nullptr;
        window_settings m_settings{};

        vk_swapchain m_swapchain{};
    };
};