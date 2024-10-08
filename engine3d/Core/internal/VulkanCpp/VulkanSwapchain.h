#pragma once
#include <cstdint>
#include <mutex>
#include <vector>
#include <vulkan/vulkan_core.h>

class GLFWwindow;
namespace engine3d::vk{

    /**
     * @name VulkanSwapchain
     * @note Implementation of a vulkan swapchain represents our frames get rendered.
     * @param p_WindowHandler is defined as our current window's swapchain.
    */
    class VulkanSwapchain{
    public:
        VulkanSwapchain() = default;
        VulkanSwapchain(GLFWwindow* p_WindowHandler, bool p_VSync);

        uint32_t GetWidth();
        uint32_t GetHeight();

        void Presentation();

    private:
        void SearchImageFormatAndColorspace();

        uint32_t AcquireNextImage();

    private:
        //! @note Per-swapchain has their own respective command buffers attached to them.
        struct VulkanSwapchainCommandBuffer{
            VkCommandPool CommandBufferPool = nullptr;
            VkCommandBuffer CommndBufferData = nullptr;
        };

        struct SwapchainImage{
            VkImage image = nullptr;
            VkImageView image_view = nullptr;
        };

        std::vector<VulkanSwapchainCommandBuffer> m_CommandBuffers;
        std::vector<SwapchainImage> m_SwapchainImages;
        std::vector<VkImage> m_Images;

        //! @note Semaphores to indicate when the images are available for rendering
        std::vector<VkSemaphore> m_ImageAvailableSemaphores;

        //! @note Semaphores for indicating when images that are rendering has been finished (pairing for each frames-in-flight)
        std::vector<VkSemaphore> m_ImagesFinishedRenderingSemaphores;
        std::vector<VkFramebuffer> m_Framebuffers;
        std::vector<VkFence> m_Fences;

        uint32_t m_Width, m_Height;
        uint32_t m_queue_node_idx;

        VkRenderPass m_RenderPassSwapchain = nullptr;
        VkSurfaceKHR m_Surface = nullptr;
        bool m_VSync;
        VkFormat m_ColorFormat;
        VkColorSpaceKHR  m_ColorSpace;

        uint32_t m_CurrentFrameIdx = 0;

        VkSwapchainKHR m_CurrentSwapchain, m_OldSwapchain;

        bool m_DoLockForCompute=false;
        std::mutex m_GraphicsMutex, m_ComputeMutex;
    };
};