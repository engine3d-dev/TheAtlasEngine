module;

#include <GLFW/glfw3.h>
#include <cstdint>

export module vulkan:window_context;

import core;
import atlas.logger;
import atlas.window;

export namespace atlas {
    namespace vulkan {
        /**
         * @brief vulkan-backend implementation of the application-window
        */
        class window_context : public window {
        public:
            window_context(const window_params& p_params) : m_params(p_params) {
                if(!glfwVulkanSupported()) {
                    console_log_error("GLFW: Vulkan is not supported!!!");
                    return;
                }

                glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
                glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

                m_window_handle = glfwCreateWindow(static_cast<int>(m_params.width), static_cast<int>(m_params.height), m_params.name.c_str(), nullptr, nullptr);

                glfwMakeContextCurrent(m_window_handle);
            }

            virtual ~window_context() = default;


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
            window_params m_params;
        };
    };
};


// /*




// core: module that is partitioned associated with
//     logger
//     import logger;
// drivers:
//     vulkan: module that is partitioned associated with
//         import vulkan;
//             internal: export module vulkan:*
//     jolt_cpp: module that is partitioned associated with
//         import jolt_cpp
//             internal: export module jolt_cpp:*


// atlas: global module unit
//     export import core;
//     export import vulkan;
//     export import jolt_cpp


// */