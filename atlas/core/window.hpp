#pragma once
#include <GLFW/glfw3.h>
#include <core/core.hpp>
#include <vulkan/vulkan.h>
#include <core/utilities/types.hpp>
#include <drivers/vulkan-cpp/vk_swapchain.hpp>

namespace atlas {

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
        [[nodiscard]] vk::vk_swapchain current_swapchain() const {
            return window_swapchain();
        }

        operator GLFWwindow*() const { return native_window(); }

        operator GLFWwindow*() { return native_window(); }

        /**
         * @brief Closing the window operation
         */
        void close();

        /**
         * @param p_current_frame_idx is the current frame index for the next available image
        */
        void present(const uint32_t& p_current_frame_idx);

    private:
        [[nodiscard]] virtual window_settings settings() const = 0;
        [[nodiscard]] virtual GLFWwindow* native_window() const = 0;
        [[nodiscard]] virtual uint32_t read_acquired_next_frame() = 0;
        [[nodiscard]] virtual vk::vk_swapchain window_swapchain() const = 0;
        virtual void presentation_process(const uint32_t& p_current_frame) = 0;
    };

    ref<window> create_window(const window_settings& p_settings);
};