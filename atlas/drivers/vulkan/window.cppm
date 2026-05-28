module;

#include <string>
#include <cstdint>
#include <print>
#include <memory>

#define GLFW_INCLUDE_VULKAN
#if _WIN32
#define VK_USE_PLATFORM_WIN32_KHR
#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>
#include <vulkan/vulkan.h>
#else
#include <GLFW/glfw3.h>
#include <vulkan/vulkan.h>
#endif

#include <GLFW/glfw3.h>
#include <span>

// export module atlas.window;
export module atlas.drivers.vulkan:window;

import atlas.common;
import atlas.core.utilities;
import vk;
import :graphics_context;

export namespace atlas {

    /*
    class window {
    public:
        virtual ~window() = default;

        [[nodiscard]] window_params data() const { return get_params(); }

        // **
        //  * @brief Checks if window is available to close
        //  *
        [[nodiscard]] bool available() const {
            return !glfwWindowShouldClose(native_window());
        }

        //  *
        //  * @brief Returns the aspect ratio of the current window
        //  *
        [[nodiscard]] float aspect_ratio() const {
            return static_cast<float>(get_params().width) /
                   static_cast<float>(get_params().height);
        }

        //  *
        //  * @brief gives you the next presentable image to use and the index
    to
        //  * retrieving that image
        //  *
        //  * @return uint32_t
        //  *
        [[nodiscard]] uint32_t acquired_next_frame() {
            return read_acquired_next_frame();
        }

        // **
        //  * @brief Returns the window's currently selected swapchain
        //  *
        [[nodiscard]] vulkan::swapchain current_swapchain() const {
            return window_swapchain();
        }

        //  *
        //  * @brief retrieves the current command buffer using the current
    frame
        //  * index to ensure we are processing commands to the right command
        //  * buffer in our current frame
        //  *
        //  * @return command buffer to actively record commands to
        //  *
        vk::command_buffer active_command(uint32_t p_frame_index) {
            return current_active_command(p_frame_index);
        }

        //  *
        //  * @brief operator overload for treating atlas::window as a
    GLFWwindow
        //  * handle
        //  *
        operator GLFWwindow*() const { return native_window(); }

        //  *
        //  * @brief operator overload for treating atlas::window as a
    GLFWwindow
        //  * handle
        //  *
        operator GLFWwindow*() { return native_window(); }

        //  *
        //  * @brief Closing the window operation
        //  *
        void close() { glfwSetWindowShouldClose(native_window(), true); }

        //  *
        //  * @brief does the presentation operation that is operated internally
        //  * with the vulkan swapchain
        //  *
        //  * @param p_current_frame_idx is current frame index to currently
        //  * process an image in the current frame
        //  *
        void present(const uint32_t& p_current_frame_idx) {
            return present_frame(p_current_frame_idx);
        }

    protected:
        [[nodiscard]] virtual window_params get_params() const = 0;
        [[nodiscard]] virtual GLFWwindow* native_window() const = 0;
        [[nodiscard]] virtual uint32_t read_acquired_next_frame() = 0;
        [[nodiscard]] virtual vulkan::swapchain window_swapchain() const = 0;

        [[nodiscard]] virtual vk::command_buffer current_active_command(
          uint32_t p_frame_idx) = 0;

        virtual void present_frame(const uint32_t& p_current_frame) = 0;
    };
    */

    class window {
    public:
        window() = default;
        window(std::shared_ptr<graphics_context> p_context, const window_params& p_params)
          : m_device(p_context->logical_device()) {
            m_instance = p_context->instance_handle();

            glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
            glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
            m_params = p_params;
            
            m_window = glfwCreateWindow(static_cast<int>(p_params.width),
                                        static_cast<int>(p_params.height),
                                        p_params.name.c_str(),
                                        nullptr,
                                        nullptr);

            glfwMakeContextCurrent(m_window);
            std::println("Constructing atlas::window");

            m_surface = std::make_shared<vk::surface>(p_context->instance_handle(), m_window);
            
            center_window();
            vk::swapchain_params swapchain_params = {
                .width = static_cast<uint32_t>(m_params.width),
                .height = static_cast<uint32_t>(m_params.height),
                .present_index = 0,
            };

            const VkSurfaceKHR surface = *m_surface;
            m_surface_properties = p_context->physical_device().request_surface(surface);
            m_swapchain = std::make_shared<vk::swapchain>(*p_context->logical_device(), surface, swapchain_params, p_context->physical_device().request_surface(surface));

            vk::queue_params present_params = {
                .family = 0,
                .index = 0,
            };
            m_present_queue = vk::device_present_queue(*p_context->logical_device(), *m_swapchain, present_params);

            // graphics_context::submit_resource_free([this](){
            //     std::println("atlas::window submit resource free");
            //     // m_swapchain.destruct();
            // });
        }

        ~window() {
            // At the end of the lifetime of this window, we can destroy any
            // resources that any of the concurrent resources this window
            // owns is no longer using
            m_surface->destruct();
            glfwDestroyWindow(m_window);
        }

        [[nodiscard]] std::span<const VkImage> request_images() const {
            return m_swapchain->get_images();
        }

        [[nodiscard]] vk::surface_params surface_properties() const { return m_surface_properties; }


        [[nodiscard]] uint32_t acquire_next_frame() {
            return m_present_queue.acquire_next_image();
        }

        void destruct() {
            // Any dependent resources that must be invoked
            // before the primary resources like VkSurfaceKHR
            // are destructed
            m_swapchain->destruct();
            m_present_queue.destruct();
        }

        GLFWwindow* glfw_window() const { return m_window; }

        void center_window() {
            GLFWmonitor* monitor = glfwGetPrimaryMonitor();
            const GLFWvidmode* mode = glfwGetVideoMode(monitor);
            uint32_t width = (mode->width / 2) - (m_params.width / 2);
            uint32_t height = (mode->height / 2) - (m_params.height / 2);
            glfwSetWindowPos(m_window,
                             static_cast<int>(width),
                             static_cast<int>(height));
        }

        void submit(std::span<const VkCommandBuffer> p_commands) {
            m_present_queue.submit_async(p_commands);
        }

        void present(uint32_t p_frame_idx) {
            m_present_queue.present_frame(p_frame_idx);
        }

        [[nodiscard]] bool available() const {
            return !glfwWindowShouldClose(m_window);
        }

    private:
        vk::instance m_instance;
        std::shared_ptr<vk::device> m_device=nullptr;
        std::shared_ptr<vk::surface> m_surface;
        std::shared_ptr<vk::swapchain> m_swapchain;
        vk::surface_params m_surface_properties;
        vk::device_present_queue m_present_queue;
        GLFWwindow* m_window = nullptr;
        window_params m_params{};
    };
};