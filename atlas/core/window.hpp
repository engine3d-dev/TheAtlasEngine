#pragma once
#include <GLFW/glfw3.h>
#include <core/core.hpp>
#include <drivers/swapchain.hpp>
#include <string>

namespace atlas {

    class window {
    public:
        virtual ~window() = default;
        static ref<window> create(
          uint32_t p_width,
          uint32_t p_height,
          const std::string& p_tag = "Undefined Window Title");

        [[nodiscard]] GLFWwindow* get_native_window() const;

        [[nodiscard]] uint32_t get_width() const;
        [[nodiscard]] uint32_t get_height() const;

        [[nodiscard]] bool is_active() const;

        [[nodiscard]] float get_aspect_ratio() const;

        ref<swapchain> get_current_swapchain();

        //! @note This treats our window class to be able to get called like a
        //! "GLFWwindow*"
        operator GLFWwindow*() { return get_native_window(); }

        void close();

    private:
        [[nodiscard]] virtual uint32_t read_width() const = 0;
        [[nodiscard]] virtual uint32_t read_height() const = 0;
        [[nodiscard]] virtual GLFWwindow* native_window() const = 0;
        virtual ref<swapchain> current_swapchain() = 0;
    };
};