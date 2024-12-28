#include "application_instance.hpp"
#include <drivers/vulkan/vulkan-imports.hpp>
#include <drivers/vulkan/vulkan_window.hpp>
#include <core/engine_logger.hpp>
#include <drivers/vulkan/helper_functions.hpp>
#include <drivers/vulkan/vulkan_context.hpp>
#include <imgui/backends/imgui_impl_glfw.h>

namespace engine3d::vk{
    static uint32_t g_Width = 0;
    static uint32_t g_Height = 0;

    VulkanWindow::VulkanWindow(uint32_t p_Width, uint32_t p_Height, const std::string& p_Tag){
        ConsoleLogInfo("Vulkan Window Begin Initialization!");
        if(!glfwInit()){
            ConsoleLogWarn("GLFW: Failed to initialize");
            return;
        }

        if(!glfwVulkanSupported()){
            ConsoleLogWarn("GLFW: Vulkan is not supported!");
            return;
        }

        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

        m_WindowHandler = glfwCreateWindow((int)p_Width, (int)p_Height, p_Tag.c_str(), nullptr, nullptr);
        glfwMakeContextCurrent(m_WindowHandler);
        ConsoleLogWarn("Vulkan Window End Initialization Completed!!");
        
        ConsoleLogInfo("Vulkan Window: Begin Initializing Window Surface!");
        vk_check(glfwCreateWindowSurface(VulkanContext::GetVkInstance(), m_WindowHandler, nullptr, &m_Surface), "glfwCreateWindowSurface", __FILE__, __LINE__, __FUNCTION__);
        ConsoleLogWarn("glfwCreateWindowSurface Initialized!");

        ConsoleLogInfo("Vulkan: Swapchain Begin Initialization!");
        m_Swapchain = Swapchain::InitializeSwapchain(m_Surface);
        ConsoleLogWarn("Vulkan: Swapchain End Initialization!");

        glfwSetWindowUserPointer(m_WindowHandler, this);
        glfwSetWindowSizeCallback(m_WindowHandler, OnWindowResize);
        // glfwSetFramebufferSizeCallback(m_WindowHandler, OnWindowResize);

        // ConsoleLogInfo("Before ImGui_ImplGlfw_InitForVulkan was called!");
        // ImGui_ImplGlfw_InitForVulkan(m_WindowHandler, true);
    }

    uint32_t VulkanWindow::Width() const {
        return g_Width;
    }

    uint32_t VulkanWindow::Height() const {
        return g_Height;
    }

    uint32_t VulkanWindow::FocusedWidth() const {
        return g_Width;
    }

    uint32_t VulkanWindow::FocusedHeight() const {
        return g_Height;
    }

    void VulkanWindow::OnWindowResize(GLFWwindow* Window, int Width, int Height){
        auto window = static_cast<GLFWwindow*>(glfwGetWindowUserPointer(Window));
        g_Width = Width;
        g_Height = Height;
        if(g_Width != ApplicationInstance::GetWindow().GetWidth() || g_Height != ApplicationInstance::GetWindow().GetHeight()){
            ApplicationInstance::GetWindow().GetCurrentSwapchain()->SetResizeStatus(true);
        }
    }

    GLFWwindow* VulkanWindow::NativeWindow() const { return m_WindowHandler; }
};