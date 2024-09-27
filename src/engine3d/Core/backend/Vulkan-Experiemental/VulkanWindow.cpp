#include <Core/backend/Vulkan-Experiemental/VulkanWindow.h>
#include <backend/Vulkan-Experiemental/VulkanSwapchain.h>

namespace engine3d::vk{
    struct VulkanWindowProperties{
        std::string title;
        uint32_t width, height;
    };

    static VulkanWindowProperties g_WindowProperties{};

    VulkanWindow::VulkanWindow(const uint32_t& p_Width, const uint32_t p_Height, const std::string& p_Title){
        g_WindowProperties.title = p_Title;
        g_WindowProperties.width = p_Width;
        g_WindowProperties.height = p_Height;

        //! @note initializes our swapchain and our VkImage/VkImageView's
        // vk::VulkanSwapchain::InitializeSwaphchain();
    }

    std::string VulkanWindow::GetTitle() const { return g_WindowProperties.title; }

    uint32_t VulkanWindow::GetWidth() const { return g_WindowProperties.width; }

    uint32_t VulkanWindow::GetHeight() const { return g_WindowProperties.height; }

    VkSwapchainKHR& VulkanWindow::CurrentWindowSwapchain() const{}

    void VulkanWindow::Presentation(){}
};
