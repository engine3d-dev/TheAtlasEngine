#include <GLFW/glfw3.h>
#include <core/application.hpp>
#include <core/engine_logger.hpp>
#include <core/window.hpp>
#include <string>
#include <drivers/vulkan/vulkan_window.hpp>

namespace atlas {
    static uint32_t g_width = -1;
    static uint32_t g_height = -1;

    ref<window> window::create(uint32_t p_width,
                               uint32_t p_height,
                               const std::string& p_tag) {
        g_width = p_width;
        g_height = p_height;
        switch (application::current_api()) {
            case VULKAN:
                return create_ref<vk::vk_window>(p_width, p_height, p_tag);
            default:
                console_log_error(
                  "API that was input was not specifiying valid backend!");
                return nullptr;
        }

        return nullptr;
    }

    ref<swapchain> window::get_current_swapchain() {
        return current_swapchain();
    }

    uint32_t window::get_width() const {
        return g_width;
    }

    uint32_t window::get_height() const {
        return g_height;
    }

    bool window::is_active() const {
        return !glfwWindowShouldClose(get_native_window());
    }

    void window::close() {
        glfwSetWindowShouldClose(get_native_window(), true);
    }

    float window::get_aspect_ratio() const {
        return (float)get_width() / (float)get_height();
    }

    GLFWwindow* window::get_native_window() const {
        return native_window();
    }
};