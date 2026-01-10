module;

#include <GLFW/glfw3.h>
#include <string>
#include <cstdint>

export module core:window;

export import :common;
export import :api;

export namespace atlas {
    struct window_params {
        uint32_t width;
        uint32_t height;
        std::string name="";
    };

    class window {
    public:
        virtual ~window() = default;

        /**
         * @brief Returns the width dimension of the window
         */
        [[nodiscard]] uint32_t width() const;

        /**
         * @brief Returns the height dimension of the window
         */
        [[nodiscard]] uint32_t height() const;

        /**
         * @brief Checks if window is available to close
         */
        [[nodiscard]] bool available() const;

        /**
         * @brief Returns the aspect ratio of the current window
         */
        [[nodiscard]] float aspect_ratio() const;

        /**
         * @brief gives you the next presentable image to use and the index to
         * retrieving that image
         *
         * @return uint32_t
         */
        [[nodiscard]] uint32_t acquired_next_frame() {
            return read_acquired_next_frame();
        }

        /**
         * @brief Returns the window's currently selected swapchain
         */
        // [[nodiscard]] vk::vk_swapchain current_swapchain() const {
        //     return window_swapchain();
        // }

        /**
         * @brief retrieves the current command buffer using the current frame
         * index to ensure we are processing commands to the right command
         * buffer in our current frame
         *
         * @return command buffer to actively record commands to
         */
        // ::vk::command_buffer active_command(uint32_t p_frame_index) {
        //     return current_active_command(p_frame_index);
        // }

        /**
         * @brief operator overload for treating atlas::window as a GLFWwindow
         * handle
         */
        operator GLFWwindow*() const { return native_window(); }

        /**
         * @brief operator overload for treating atlas::window as a GLFWwindow
         * handle
         */
        operator GLFWwindow*() { return native_window(); }

        /**
         * @brief Closing the window operation
         */
        void close();

        /**
         * @brief does the presentation operation that is operated internally
         * with the vulkan swapchain
         *
         * @param p_current_frame_idx is current frame index to currently
         * process an image in the current frame
         */
        void present(const uint32_t& p_current_frame_idx);

    protected:
        // [[nodiscard]] virtual window_settings settings() const = 0;
        [[nodiscard]] virtual GLFWwindow* native_window() const = 0;
        [[nodiscard]] virtual uint32_t read_acquired_next_frame() = 0;
        // [[nodiscard]] virtual vk::vk_swapchain window_swapchain() const = 0;

        // [[nodiscard]] virtual ::vk::command_buffer current_active_command(uint32_t p_frame_idx) = 0;

        virtual void presentation_process(const uint32_t& p_current_frame) = 0;
    };

    /**
     * @brief constructs an atlas::window
     *
     * There should only ever be one window constructed throughout the entire
     * application
     *
     * @param p_settings is the window settings to construct the window with
     *
     * @return shared_ptr<atlas::window>
     */
    ref<window> create_window(const window_params& p_params, graphics_api p_api) {
        return nullptr;
    }
};