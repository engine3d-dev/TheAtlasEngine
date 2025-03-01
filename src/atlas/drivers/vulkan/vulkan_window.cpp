#include "application_instance.hpp"
#include <drivers/vulkan/vulkan-imports.hpp>
#include <drivers/vulkan/vulkan_window.hpp>
#include <core/engine_logger.hpp>
#include <drivers/vulkan/helper_functions.hpp>
#include <drivers/vulkan/vulkan_context.hpp>
#include <imgui/backends/imgui_impl_glfw.h>

namespace atlas::vk{
    static uint32_t g_Width = 0;
    static uint32_t g_Height = 0;

    vk_window::vk_window(uint32_t p_Width, uint32_t p_Height, const std::string& p_tag){
        console_log_info("Vulkan Window Begin Initialization!");
        if(!glfwInit()){
            console_log_warn("GLFW: Failed to initialize");
            return;
        }

        if(!glfwVulkanSupported()){
            console_log_warn("GLFW: Vulkan is not supported!");
            return;
        }

        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

        m_window = glfwCreateWindow((int)p_Width, (int)p_Height, p_tag.c_str(), nullptr, nullptr);
        glfwMakeContextCurrent(m_window);
        console_log_warn("Vulkan Window End Initialization Completed!!");
        
        console_log_info("Vulkan Window: Begin Initializing Window Surface!");
        vk_check(glfwCreateWindowSurface(vk_context::get_vk_instance(), m_window, nullptr, &m_surface), "glfwCreateWindowSurface", __FILE__, __LINE__, __FUNCTION__);
        console_log_warn("glfwCreateWindowSurface Initialized!");

        console_log_info("Vulkan: swapchain Begin Initialization!");
        m_swapchain = swapchain::initialize(m_surface);
        console_log_warn("Vulkan: swapchain End Initialization!");

        glfwSetWindowUserPointer(m_window, this);
        glfwSetWindowSizeCallback(m_window, on_resize);
        // glfwSetFramebufferSizeCallback(m_window, on_resize);

        // console_log_info("Before ImGui_ImplGlfw_InitForVulkan was called!");
        // ImGui_ImplGlfw_InitForVulkan(m_window, true);
    }

    uint32_t vk_window::read_width() const {
        return g_Width;
    }

    uint32_t vk_window::read_height() const {
        return g_Height;
    }

    void vk_window::on_resize(GLFWwindow* Window, int Width, int Height){
        auto window = static_cast<GLFWwindow*>(glfwGetWindowUserPointer(Window));
        if(!window){}
        
        g_Width = Width;
        g_Height = Height;
        if(g_Width != application_instance::get_window().get_width() || g_Height != application_instance::get_window().get_height()){
            application_instance::get_window().get_current_swapchain()->set_resize_status(true);
        }
    }

    GLFWwindow* vk_window::native_window() const { return m_window; }
};