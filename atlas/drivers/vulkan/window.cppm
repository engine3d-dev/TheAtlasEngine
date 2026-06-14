module;

#include <string>
#include <cstdint>
#include <memory>
#include <span>
#include <print>

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

export module atlas.drivers.vulkan:window;

import atlas.core.utilities;
import vk;
import :graphics_context;

export namespace atlas {

    class window {
    public:
        window() = default;
        window(/*NOLINT*/ std::shared_ptr<graphics_context> p_context,
               const window_params& p_params)
          : m_device(p_context->logical_device()) {
            m_instance = p_context->instance_handle();

            glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
            glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

            m_window = glfwCreateWindow(static_cast<int>(p_params.width),
                                        static_cast<int>(p_params.height),
                                        p_params.name.c_str(),
                                        nullptr,
                                        nullptr);

            glfwMakeContextCurrent(m_window);

            m_surface = std::make_shared<vk::surface>(
              p_context->instance_handle(), m_window);

            int framebuffer_width=0;
            int framebuffer_height=0;
            glfwGetFramebufferSize(m_window, &framebuffer_width, &framebuffer_height);

            // Weird issue where the frame buffer size is different then the window size.
            // Causing majority of the portion of the window to render pink pixels, indicating an error.
            m_params.width = static_cast<uint32_t>(framebuffer_width);
            m_params.height = static_cast<uint32_t>(framebuffer_height);

            std::println("Window created with extent: {}x{}", m_params.width, m_params.height);

            vk::swapchain_params swapchain_params = {
                .width = static_cast<uint32_t>(m_params.width),
                .height = static_cast<uint32_t>(m_params.height),
                .present_index = 0,
            };

            m_surface_properties =
              p_context->physical_device().request_surface(*m_surface, VK_FORMAT_B8G8R8A8_UNORM);

            m_swapchain =
              std::make_shared<vk::swapchain>(*p_context->logical_device(),
                                              *m_surface,
                                              swapchain_params,
                                              m_surface_properties);
            
            center_window();
            vk::queue_params present_params = {
                .family = 0,
                .index = 0,
            };
            m_present_queue = vk::device_present_queue(
              *p_context->logical_device(), *m_swapchain, present_params);
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

        [[nodiscard]] GLFWwindow* glfw_window() const { return m_window; }

        std::shared_ptr<vk::swapchain> swapchain_handle() {
            return m_swapchain;
        }

        [[nodiscard]] vk::device_present_queue present_queue() const {
            return m_present_queue;
        }

        [[nodiscard]] vk::surface_params surface_properties() const {
            return m_surface_properties;
        }

        void center_window() {
            int window_width=0;
            int window_height=0;
            glfwGetWindowSize(m_window, &window_width, &window_height);
            GLFWmonitor* monitor = glfwGetPrimaryMonitor();
            const GLFWvidmode* mode = glfwGetVideoMode(monitor);
            int width = (mode->width / 2) - (window_width / 2);
            int height = (mode->height / 2) - (window_height / 2);
            glfwSetWindowPos(
              m_window, width, height);
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

        [[nodiscard]] window_params extent() const {
            return m_params;
        }

    private:
        vk::instance m_instance;
        std::shared_ptr<vk::device> m_device = nullptr;
        std::shared_ptr<vk::surface> m_surface;
        std::shared_ptr<vk::swapchain> m_swapchain;
        vk::surface_params m_surface_properties;
        vk::device_present_queue m_present_queue;
        GLFWwindow* m_window = nullptr;
        window_params m_params{};
    };
};