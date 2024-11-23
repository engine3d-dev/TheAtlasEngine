#pragma once
#include <Core/Core.hpp>
#include <vulkan/vulkan_core.h>

namespace engine3d{
    class GraphicSwapchain{
    public:
        //! @note Initialize our swapchain
        static Ref<GraphicSwapchain> InitializeSwapchain(VkSurfaceKHR p_Surface);

        //! @note Public API's

        uint32_t GetImagesSize() const;

        VkFramebuffer GetFramebuffer(uint32_t index);

        VkImageView GetImageView(uint32_t index);

        VkSwapchainKHR GetVkSwapchainHandler();

        VkExtent2D GetSwapchainExtent();

        void SubmitCommandBuffer(VkCommandBuffer* p_CommandBuffers);

        //! @note Proceeds to getting next frame.
        uint32_t AcquireNextImage();

        //! @note m_CurrentFrameIdx
        uint32_t GetCurrentFramePerTick();

        VkFormat GetSwapchainFormat();

        VkRenderPass GetRenderPass();

    private:
        virtual VkSwapchainKHR VkSwapchainHandler() = 0;
        virtual VkRenderPass ReadSwapchainRenderPass() = 0;
        virtual VkFormat ReadSwapchainFormat() = 0;
        virtual uint32_t ImagesSize() const = 0;
        virtual VkFramebuffer ReadFramebuffer(uint32_t idx) = 0;
        virtual VkImageView ReadImageView(uint32_t index) = 0;
        virtual VkExtent2D ReadSwapchainExtent() = 0;
        virtual void SubmitAndWriteCommandBuffer(VkCommandBuffer* p_CommandBuffers) = 0;
        virtual uint32_t ReadAcquiredNextFrame() = 0;
        virtual uint32_t CurrentFramePerTick() = 0;

    };
};