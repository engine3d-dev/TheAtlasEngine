#pragma once
#include <core/window.hpp>
#include <GLFW/glfw3.h>
#include <drivers/vulkan-cpp/vk_swapchain.hpp>

namespace atlas::vk {
    class vk_window : public window {
    public:
        vk_window(const window_settings& p_settings);
        ~vk_window() override;

    private:
        void center_window();

    private:
        [[nodiscard]] window_settings settings() const override;
        [[nodiscard]] vk_swapchain window_swapchain() const override {
            return m_swapchain;
        }
        [[nodiscard]] uint32_t read_acquired_next_frame() override;
        [[nodiscard]] GLFWwindow* native_window() const override;
        void presentation_process(const uint32_t& p_current_frame) override;
        [[nodiscard]] vk::vk_command_buffer current_active_command_buffer(
          const uint32_t& p_frame_idx) override {
            return m_swapchain.active_command_buffer(p_frame_idx);
        }

    private:
        VkInstance m_instance_handler = nullptr;
        GLFWwindow* m_window_handler = nullptr;
        VkSurfaceKHR m_window_surface = nullptr;
        window_settings m_settings{};

        vk_swapchain m_swapchain{};
        static vk_window* s_instance;
    };
};
