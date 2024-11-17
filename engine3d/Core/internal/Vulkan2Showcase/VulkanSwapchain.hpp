#pragma once
#include <Core/GraphicDrivers/GraphicSwapchain.hpp>
#include <internal/Vulkan2Showcase/VulkanDriver.hpp>
#include <vulkan/vulkan_core.h>

namespace engine3d::vk{
    /**
     * @name Vulkan Swapchain
     * @note Defines the vulkan-backend implementation of a swapchain.
    */
    class VulkanSwapchain : public GraphicSwapchain{
    public:
        //! @note TODO: Change this into RendererConfig
        static constexpr uint32_t MaxFramesInFlight = 2;

        VulkanSwapchain() = default;
        VulkanSwapchain(VulkanPhysicalDriver p_PhysicalDriver, VulkanDriver p_Driver, VkSurfaceKHR p_Surface);

    private:
        //! @note Graphic Swapchain virtual implementation functions
        VkSwapchainKHR VkSwapchainHandler() override;
        VkRenderPass ReadSwapchainRenderPass() override;
        VkFormat ReadSwapchainFormat() override;

        uint32_t ImagesSize() const override;
        VkFramebuffer ReadFramebuffer(uint32_t idx) override;
        VkImageView ReadImageView(uint32_t index) override;
        VkExtent2D ReadSwapchainExtent() override;
        void SubmitAndWriteCommandBuffer(VkCommandBuffer* p_CommandBuffers) override;
        uint32_t ReadAcquiredNextFrame() override;
        uint32_t CurrentFramePerTick() override;

    private:
        //! @note Helper functions or various operations.
        VkPresentModeKHR SelectCompatiblePresentMode(const VkPresentModeKHR& p_RequestMode, const std::vector<VkPresentModeKHR>& p_Modes);
        VkExtent2D SelectValidExtent(const VkSurfaceCapabilitiesKHR& p_SurfaceCapabilities);

        //! @note Search Memory Type
        uint32_t SelectMemoryType(VkPhysicalDeviceMemoryProperties p_MemoryProperties, uint32_t p_TypeFilter, VkMemoryPropertyFlags property_flag);

        /**
         * @note Selecting specific type of format.
         * @param VkPhysicalDevice is used to extract the depth format and check if it is available on our current GPU.
        */
        VkFormat SelectDepthFormat(VkPhysicalDevice p_PhysicalDevice);

        //! @note Helper function to get the format types we need.
        VkFormat SelectSupportedFormat(VkPhysicalDevice p_PhysicalDevice, const std::vector<VkFormat>& p_Formats, VkImageTiling p_Tiling, VkFormatFeatureFlags p_FeatureFlag);

    private:
        VkQueue m_PresentationQueue;

        struct SwapchainImage{
            VkImage Image;
            VkImageView ImageView;
        };

        struct SwapchainDepthImage{
            VkImage Image;
            VkImageView ImageView;
            VkDeviceMemory DeviceMemory;
        };

        //! @note Images that are either in-use for images or depth images.
        std::vector<SwapchainImage> m_ImagesForSwapchain;
        std::vector<SwapchainDepthImage> m_DepthImagesForSwapchain;
        std::vector<VkFramebuffer> m_FramebuffersForSwapchain;

        VkSwapchainKHR m_Swapchain;
        VkSurfaceFormatKHR m_SurfaceFormat;
        VkPresentModeKHR m_PresentMode;
        uint32_t m_PresentationIndex;
        VkExtent2D m_SwapchainExtent;

        //! @note Setting up Command Buffers
        // VkCommandBuffer m_CommandBufferForSwapchain;
        std::vector<VkCommandBuffer> m_SwapchainCommandBuffers; // command buffers per swapchain
        VkCommandPool m_CommandPoolForSwapchain;

        VkRenderPass m_RenderpassForSwapchain;

        //! @note Semaphores to signal when frames finished or current frame being rendered has been completed.
        std::vector<VkSemaphore> m_SemaphoresForAvailableImages; // semaphores for when images are available
        std::vector<VkSemaphore> m_SemaphoresForRenderCompleted; // semaphores when working on the current frame has been completed

        // std::vector<VkFence> m_InFlightFences; // fences for when frames in flight
        // std::vector<VkFence> m_ImagesCurrentlyInFlight; // images fences for when we currently have images currently in flight.
        std::vector<VkFence> m_ImagesInFlight;
        std::vector<VkFence> m_InFlightFences;
        VulkanDriver m_Driver;

        //! @note Current frame that we are at in the application from the start of the app.
        size_t m_CurrentFrameIndex = 0;
        //! @note Acts as our image index.
        uint32_t m_CurrentImageIndex = 0; // contains the index of the image we are currently working through
    };
};