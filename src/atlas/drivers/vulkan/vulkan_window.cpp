#include <drivers/vulkan/vulkan-imports.hpp>
#include <drivers/vulkan/vulkan_window.hpp>
#include <core/engine_logger.hpp>
#include <drivers/vulkan/helper_functions.hpp>
#include <drivers/vulkan/vulkan_context.hpp>
#include <imgui/backends/imgui_impl_glfw.h>
#include <core/application.hpp>

namespace atlas::vk {
    static uint32_t g_width = 0;
    static uint32_t g_height = 0;

    vk_window::vk_window(uint32_t p_width,
                         uint32_t p_height,
                         const std::string& p_tag) {
        
        m_instance_handler = vk_context::vk_instance_handler();

        if(m_instance_handler == nullptr) {
            console_log_fatal("VkInstance == nullptr!!!");
        }
        else {
            console_log_fatal("VkInstance != nullptr!!!");
        }

        console_log_info("Vulkan Window Begin Initialization!");
        if (!glfwInit()) {
            console_log_warn("GLFW: Failed to initialize");
            return;
        }

        if (!glfwVulkanSupported()) {
            console_log_warn("GLFW: Vulkan is not supported!");
            return;
        }

        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

        m_window = glfwCreateWindow(
          (int)p_width, (int)p_height, p_tag.c_str(), nullptr, nullptr);
        glfwMakeContextCurrent(m_window);
        console_log_warn("Vulkan Window End Initialization Completed!!");

        console_log_info("Vulkan Window: Begin Initializing Window Surface!");
        vk_check(
          glfwCreateWindowSurface(
            m_instance_handler, m_window, nullptr, &m_surface),
          "glfwCreateWindowSurface",
          __FILE__,
          __LINE__,
          __FUNCTION__);
        console_log_warn("glfwCreateWindowSurface Initialized!");

        console_log_info("Vulkan: swapchain Begin Initialization!");
        m_swapchain = swapchain::initialize(m_surface);
        console_log_warn("Vulkan: swapchain End Initialization!");

        glfwSetWindowUserPointer(m_window, this);
        // glfwSetWindowSizeCallback(m_window, on_resize);
        // glfwSetFramebufferSizeCallback(m_window, on_resize);

        // console_log_info("Before ImGui_ImplGlfw_InitForVulkan was called!");
        // ImGui_ImplGlfw_InitForVulkan(m_window, true);
    }

    uint32_t vk_window::read_width() const {
        return g_width;
    }

    uint32_t vk_window::read_height() const {
        return g_height;
    }

    void vk_window::on_resize() {
        // auto window =
        //   static_cast<GLFWwindow*>(glfwGetWindowUserPointer(p_window));
        // if (!window) {
        // }

        // int w, h;
        // glfwGetFramebufferSize(m_window, &w, &h);

        // g_width = w;
        // g_height = h;
        // if (g_width != application::get_window().get_width() ||
        //     g_height != application::get_window().get_height()) {
        //     application::get_window()
        //       .get_current_swapchain()
        //       ->set_resize_status(true);
        // }
    }

    GLFWwindow* vk_window::native_window() const {
        return m_window;
    }
};