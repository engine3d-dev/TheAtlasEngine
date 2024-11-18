#include <Core/EngineLogger.hpp>
#include <internal/Vulkan2Showcase/Vulkan.hpp>
#include <internal/Vulkan2Showcase/VulkanContext.hpp>
#include <internal/Vulkan2Showcase/VulkanWindow.hpp>
#include <internal/Vulkan2Showcase/Vulkan.hpp>
#include <internal/Vulkan2Showcase/helper_functions.hpp>

namespace engine3d::vk{
    VulkanWindow::VulkanWindow(const std::string& p_Tag, uint32_t w, uint32_t h) : m_Width(w), m_Height(h){
        if(!glfwInit()){
            ConsoleLogError("glfwInit failed!");
        }

        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        m_Window = glfwCreateWindow((int)w, (int)h, p_Tag.c_str(), nullptr, nullptr);

        if(!glfwVulkanSupported()){
            ConsoleLogError("GLFW: Vulkan not supported!!!");
        }

        ConsoleLogInfo("Before Vulkan2Showcase Context Initialized!");
        vk::VulkanContext::Initialize();
        vk_check(glfwCreateWindowSurface(vk::VulkanContext::GetVkInstance(),m_Window, nullptr, &m_Surface), "glfwCreateWindowSurface", __FILE__, __LINE__, __FUNCTION__);
        ConsoleLogWarn("glfwCreateWindowSurface Initialized!");
        // m_Swapchain = VulkanSwapchain(VulkanContext::GetPhysicalDriver(), VulkanContext::GetDriver(), m_Surface);
        m_Swapchain = GraphicSwapchain::InitializeSwapchain(m_Surface);

        ConsoleLogInfo("Vulkan2Showcase VulkanWindow Initialized Complete!!!");
    }

    bool VulkanWindow::CurrentWindowActive() const { return !glfwWindowShouldClose(m_Window); }

    VkSurfaceKHR& VulkanWindow::VkSurface(){ return m_Surface; }

    GLFWwindow* VulkanWindow::NativeWindow(){ return m_Window; }

    void VulkanWindow::Presentation(){

    }
};