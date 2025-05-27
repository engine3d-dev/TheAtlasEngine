#include <GLFW/glfw3.h>
#include <core/application.hpp>
#include <core/engine_logger.hpp>
#include <core/window.hpp>
#include <drivers/vulkan-cpp/vk_window.hpp>

namespace atlas {
    ref<window> create_window(const window_settings& p_settings) {
        switch (application::current_api()) {
            case VULKAN:
                return create_ref<vk::vk_window>(p_settings);
            default:
                console_log_error(
                  "API that was input was not specifiying valid backend!");
                return nullptr;
        }

        return nullptr;
    }

    ref<swapchain> window::current_swapchain() {
        return window_current_swapchain();
    }

    uint32_t window::width() const {
        return settings().width;
    }

    uint32_t window::height() const {
        return settings().height;
    }

    bool window::available() const {
        return !glfwWindowShouldClose(native_window());
    }

    void window::close() {
        glfwSetWindowShouldClose(native_window(), true);
    }

    float window::aspect_ratio() const {
        return (float)width() / (float)height();
    }
};