#pragma once
#include <core/core.hpp>
#include <vulkan/vulkan_core.h>

namespace engine3d{
    class Swapchain{
    public:
        //! @note Initialize our swapchain
        static Ref<Swapchain> InitializeSwapchain(VkSurfaceKHR p_Surface);

        //! @note Public API's
        void OnResize(uint32_t Width, uint32_t Height);

        uint32_t GetImagesSize() const;

        VkFramebuffer GetFramebuffer(uint32_t index);

        VkImageView GetImageView(uint32_t index);

        VkSwapchainKHR GetVkSwapchainHandler();

        VkExtent2D GetSwapchainExtent();

        void SubmitCommandBuffer(VkCommandBuffer* p_CommandBuffers);

        //! @note Proceeds to getting next frame.
        uint32_t AcquireNextImage();

        //! @return m_CurrentFrameIdx
        uint32_t GetCurrentFramePerTick();

        VkFormat& GetSwapchainFormat();

        VkRenderPass GetRenderPass();

        bool IsResized();
        void SetResizeStatus(bool IsResizeSignaled);

    private:
        virtual void ResizeStatus(bool) = 0;
        virtual bool SwapchainResized() = 0;
        virtual void RecreateSwapchain(uint32_t Width, uint32_t Height) = 0;
        virtual VkSwapchainKHR VkSwapchainHandler() = 0;
        virtual VkRenderPass ReadSwapchainRenderPass() = 0;
        virtual VkFormat& ReadSwapchainFormat() = 0;
        virtual uint32_t ImagesSize() const = 0;
        virtual VkFramebuffer ReadFramebuffer(uint32_t idx) = 0;
        virtual VkImageView ReadImageView(uint32_t index) = 0;
        virtual VkExtent2D ReadSwapchainExtent() = 0;
        virtual void SubmitAndWriteCommandBuffer(VkCommandBuffer* p_CommandBuffers) = 0;
        virtual uint32_t ReadAcquiredNextFrame() = 0;
        virtual uint32_t CurrentFramePerTick() = 0;

    };
};