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
        m_Swapchain = VulkanSwapchain(VulkanContext::GetPhysicalDriver(), VulkanContext::GetDriver(), m_Surface);

        ConsoleLogInfo("Vulkan2Showcase VulkanWindow Initialized Complete!!!");
    }

    bool VulkanWindow::CurrentWindowActive() const { return !glfwWindowShouldClose(m_Window); }

    VkSurfaceKHR& VulkanWindow::VkSurface(){ return m_Surface; }

    VkRenderPass& VulkanWindow::VkRenderpass(){
        return m_Swapchain.GetRenderpass();
    }

    VkFramebuffer VulkanWindow::ReadFramebufferAt(uint32_t index){
        return m_Swapchain.GetFramebuffer(index);
    }

    uint32_t VulkanWindow::NextImagePerFrame(){
        return m_Swapchain.AcquireNextImage();
    }

    VkSwapchainKHR VulkanWindow::VkSwapchain() {
        return m_Swapchain.GetVkSwapchainHandler();
    }

    uint32_t VulkanWindow::PerFrameTick() {
        return m_Swapchain.GetCurrentPerFrameTick();
    }

    void VulkanWindow::SubmitCommandBufferToSwapchain(VkCommandBuffer* p_CommandBuffers){
        m_Swapchain.SubmitCommandBuffers(p_CommandBuffers);
    }

    void VulkanWindow::SubmitCommandBufferToSwapchain(VkCommandBuffer* p_CommandBuffers, uint32_t& image_index){
        m_Swapchain.SubmitCommandBuffers(p_CommandBuffers, image_index);
    }

    // Ref<Swapchain> VulkanWindow::CurrentSwapchain() {
        // return m_Swapchain;
    // }

    GLFWwindow* VulkanWindow::NativeWindow(){ return m_Window; }

    void VulkanWindow::Presentation(){

    }
};