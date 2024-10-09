#include <internal/VulkanCpp/VulkanDevice.hpp>
#include <Core/internal/VulkanCpp/VulkanWindow.hpp>
#include <Core/internal/VulkanCpp/Vulkan.hpp>
#include <stdexcept>
#include <vulkan/vulkan_core.h>

#if _WIN32
#define VK_USE_PLATFORM_WIN32_KHR
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>
#include <vulkan/vulkan.h>
#else
#include <GLFW/glfw3.h>
#include <vulkan/vulkan.h>
#endif

namespace engine3d::vk{
    VulkanWindow::VulkanWindow(uint32_t p_Width, uint32_t p_Height, const std::string& p_Title) : m_Width(p_Width), m_Height(p_Height), m_Title(p_Title) {
        if(!glfwInit()){
            throw std::runtime_error("glfwInit() failed to initialize!");
        }

        if(!glfwVulkanSupported()){
            throw std::runtime_error("glfwVulkanSupported() failed because vulkan might not be supported");
        }

        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

        m_Window = glfwCreateWindow(p_Width, p_Height, p_Title.c_str(), nullptr, nullptr);
        glfwMakeContextCurrent(m_Window);

        VkResult res = glfwCreateWindowSurface(Vulkan::GetVkInstance(), m_Window, nullptr, &m_Surface);
        if(res != VK_SUCCESS){
            throw std::runtime_error("glfwCreateWindowSurface() failed error number: =====> " + std::to_string(res));
        }
        m_IsCurrentWindowActive = true;
    }

    void VulkanWindow::UpdatePerTick(){
        glfwPollEvents();
    }

    void VulkanWindow::Presentation(){
        // m_CurrentWindowSwapchain->Presentation();
    }

    bool VulkanWindow::CurrentWindowActive() const { return !glfwWindowShouldClose(m_Window); }

    VkSurfaceKHR& VulkanWindow::VkSurface() { return m_Surface; }
    GLFWwindow* VulkanWindow::NativeWindow() { return m_Window; }

    uint32_t VulkanWindow::Width() const{ return m_Width; }

    uint32_t VulkanWindow::Height() const{ return m_Height; }

    std::string VulkanWindow::Title() const{ return m_Title; }
};
