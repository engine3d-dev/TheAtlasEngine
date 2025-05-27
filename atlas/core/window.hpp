#pragma once
#include <GLFW/glfw3.h>
#include <core/core.hpp>
#include <drivers/swapchain.hpp>
#include <string>

namespace atlas {

    /**
     * @brief Specific settings to the window configuration
     * @brief Contains window properties for doing things with the windows
    */
    struct window_settings {
        uint32_t width = -1;
        uint32_t height = -1;
        std::string name = "";
        uint32_t frames_in_flight = 2;
    };

    class window {
    public:
        virtual ~window() = default;

        //! @brief Returns the width dimension of the window
        [[nodiscard]] uint32_t width() const;

        //! @brief Returns the height dimension of the window
        [[nodiscard]] uint32_t height() const;

        /**
         * @brief Checks if window is available to close
         */
        [[nodiscard]] bool available() const;

        //! @brief Returns the aspect ratio of the current window
        [[nodiscard]] float aspect_ratio() const;

        /**
         * @brief Returns the available presentable image to use, retrieve index
         * that image is called
         */
        [[nodiscard]] uint32_t acquired_next_frame() {
            return read_acquired_next_frame();
        }

        /**
         * @brief Returns the window's currently selected swapchain
         */
        ref<swapchain> current_swapchain();

        operator GLFWwindow*() const { return native_window(); }

        operator GLFWwindow*() { return native_window(); }

        /**
         * @brief Closing the window operation
         */
        void close();

    private:
        [[nodiscard]] virtual window_settings settings() const = 0;
        [[nodiscard]] virtual GLFWwindow* native_window() const = 0;
        [[nodiscard]] virtual uint32_t read_acquired_next_frame() = 0;
        virtual ref<swapchain> window_current_swapchain() = 0;
    };

    ref<window> create_window(const window_settings& p_settings);
};