module;

#include <cstdint>

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

export module atlas.drivers.vulkan.window_context;

import atlas.window;
import atlas.logger;
import atlas.drivers.vulkan.instance_context;
import atlas.drivers.vulkan.utilities;

export namespace atlas {
    namespace vulkan {
        /**
         * @brief vulkan-backend implementation of the application-window
        */
        class window_context : public window {
        public:
            window_context(const window_params& p_params) : m_params(p_params) {

                console_log_info("window_context constructed!!!");

                if(!glfwVulkanSupported()) {
                    console_log_error("GLFW: Vulkan is not supported!!!");
                    console_log_error("GLFW: Vulkan Supported = {}", static_cast<bool>(glfwVulkanSupported()));
                    return;
                }

                glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
                glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

                m_instance = instance_context::handle();

                m_window_handle = glfwCreateWindow(static_cast<int>(m_params.width), static_cast<int>(m_params.height), m_params.name.c_str(), nullptr, nullptr);

                glfwMakeContextCurrent(m_window_handle);

                console_log_info("m_instance = {}", (m_instance == nullptr));

                vk_check(
                    glfwCreateWindowSurface(
                    m_instance, m_window_handle, nullptr, &m_window_surface),
                    "glfwCreateWindowSurface");

                center_window();
            }

            virtual ~window_context() {
                if(m_window_surface != nullptr) {
                    vkDestroySurfaceKHR(m_instance, m_window_surface, nullptr);
                }
                glfwDestroyWindow(m_window_handle);
            }


        protected:
            [[nodiscard]] window_params get_params() const override {
                return m_params;
            }
            
            [[nodiscard]] GLFWwindow* native_window() const override {
                return m_window_handle;
            }

            [[nodiscard]] uint32_t read_acquired_next_frame() override {
                return 0;
            }

            void present_frame(const uint32_t& p_current_frame) override {
            }

        private:
            void center_window() {
                GLFWmonitor* monitor = glfwGetPrimaryMonitor();
                const GLFWvidmode* mode = glfwGetVideoMode(monitor);
                uint32_t width = (mode->width / 2) - (m_params.width / 2);
                uint32_t height = (mode->height / 2) - (m_params.height / 2);
                glfwSetWindowPos(m_window_handle, static_cast<int>(width), static_cast<int>(height));
            }

        private:
            GLFWwindow* m_window_handle=nullptr;
            VkSurfaceKHR m_window_surface=nullptr;
            window_params m_params;
            VkInstance m_instance=nullptr;
        };
    };
};