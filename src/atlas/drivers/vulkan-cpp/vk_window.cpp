#include <drivers/vulkan-cpp/vulkan-imports.hpp>
#include <drivers/vulkan-cpp/vk_window.hpp>
#include <core/engine_logger.hpp>
#include <drivers/vulkan-cpp/vk_context.hpp>
#include <drivers/vulkan-cpp/helper_functions.hpp>

namespace atlas::vk {
	vk_window* vk_window::s_instance = nullptr;
    vk_window::vk_window(const window_settings& p_settings)
      : m_settings(p_settings) {

        if (!glfwVulkanSupported()) {
            console_log_warn("GLFW: Vulkan is not supported!");
            return;
        }

        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

        m_instance_handler = vk_context::handler();

        m_window_handler = glfwCreateWindow((int)p_settings.width,
                                            (int)p_settings.height,
                                            p_settings.name.c_str(),
                                            nullptr,
                                            nullptr);

        glfwMakeContextCurrent(m_window_handler);

        vk_check(
          glfwCreateWindowSurface(
            m_instance_handler, m_window_handler, nullptr, &m_window_surface),
          "glfwCreateWindowSurface",
          __FILE__,
          __LINE__,
          __FUNCTION__);

        center_window();

        m_swapchain = vk_swapchain(m_window_surface, m_settings);

        vk_context::submit_resource_free([this](){
			console_log_fatal("vk_window submit freed resources!!!");
			m_swapchain.destroy();
        });

        glfwSetWindowUserPointer(m_window_handler, this);

		s_instance = this;
    }

	vk_window::~vk_window() {
		vkDestroySurfaceKHR(m_instance_handler, m_window_surface, nullptr);
		vkDestroyInstance(m_instance_handler, nullptr);
		glfwDestroyWindow(m_window_handler);
	}

    void vk_window::center_window() {
        GLFWmonitor* monitor = glfwGetPrimaryMonitor();
        const GLFWvidmode* mode = glfwGetVideoMode(monitor);
        uint32_t width = (mode->width / 2) - (m_settings.width / 2);
        uint32_t height = (mode->height / 2) - (m_settings.height / 2);
        glfwSetWindowPos(m_window_handler, (int)width, (int)height);
    }

	void vk_window::presentation_process(const uint32_t& p_current_frame) {
		m_swapchain.present(p_current_frame);
	}

    window_settings vk_window::settings() const {
        return m_settings;
    }

    uint32_t vk_window::read_acquired_next_frame() {
        return m_swapchain.read_acquired_image();
    }

    GLFWwindow* vk_window::native_window() const {
        return m_window_handler;
    }
};