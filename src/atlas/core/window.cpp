#include <GLFW/glfw3.h>
#include <core/application.hpp>
#include <core/engine_logger.hpp>
#include <core/window.hpp>
#include <string>

// #include <imgui/backends/imgui_impl_glfw.h>
#include <drivers/vulkan/vulkan_window.hpp>

namespace atlas {
    static uint32_t g_WindowWidth = -1;
    static uint32_t g_WindowHeight = -1;

    ref<window> window::create(uint32_t Width,
                               uint32_t Height,
                               const std::string& Tag) {
        g_WindowWidth = Width;
        g_WindowHeight = Height;
        switch (application::current_api()) {
            case VULKAN:
                return create_ref<vk::vk_window>(Width, Height, Tag);
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
        return g_WindowWidth;
    }

    uint32_t window::get_height() const {
        return g_WindowHeight;
    }

    bool window::is_active() const {
        return !glfwWindowShouldClose(get_native_window());
    }

    void window::close() {
        // glfwDestroyWindow(get_native_window());
        glfwSetWindowShouldClose(get_native_window(), true);
    }

    float window::get_aspect_ratio() const {
        return (float)get_width() / get_height();
    }

    GLFWwindow* window::get_native_window() const {
        return native_window();
    }
};