#pragma once
#include <vulkan/vulkan.h>
#include <vector>
#include <vulkan/vulkan_core.h>
#include <Core/backend/Vulkan/Shaders/VulkanShaderPipelineBuilder.h>

namespace engine3d{
    namespace vk{
        /**
         * @name VulkanSwapchain
         * @note TODO --- Make this part of the swap chain base class as we may want to enable devs to switch from vulkan to directx11/12 in the future.
         * @note Abstraction for how Vulkan handles swapchain.
        */

        /**
         * @name Vulkan Swap Chain
         * @note Swap chains allow us to do things like shadow and lighting passes for deferred rendering
         * @note moving images from the actual application to display.
         * @note Handles format and presentation modes, containing multiple images before displaying to the actual display from application.
         * @note Vulkan supports double-buffers which just means switching from back and front buffers.
         * @note Switching frames are referred to in Vulkan as presentation modes
         * 
         * @note Multiple Swapchains
         * @note Typically you only have one swap chain per monitor/screen.
         * @note The cases are having multiple swapchains is if you were to have multiple monitors that you want to present to.
         * 
         * @note Here are some API calls that im considering for swapchains
        vk::VulkanSwapchain m_Swapchain = vk::Swapchain();

        @note This means that we are reading in a `VkFramebuffer`
        @note Decision is because what if we want to create our framebuffer but also gives us control on what we do with our swapchain.
         auto res = read_framebuffer(&m_Swapchain, index);

        */

        class VulkanSwapchain{
        public:
            ~VulkanSwapchain();

            //! @note Initiating our vulkan swapchain.
            static void InitializeSwaphchain();

            //! @note Reading the size of our images our swapchain has
            static uint32_t GetImagesSize();

            //! @note Reading our image from our swapchain
            static VkImage& GetImage(uint32_t index);
            static VkImageView& GetImageView(uint32_t index);

            //! @note Reading the swapchain handler
            static VkSwapchainKHR& GetVkSwapchainInstance();

            //! @note Reading the format and render pass from the vulkan swapchain
            static VkFormat& GetSwapchainFormat();
            static VkRenderPass& GetSwapchainRenderPass();

            //! @note TODO -- move out the framebuffer logic outside of VulkanSwapchain
            //! @note Setting our current framebuffer 
            static void SetCurrentFramebuffer(uint32_t index);

            static VkFramebuffer& ReadFramebuffer(uint32_t FrameIndex);

            //! @note TODO -- Moving the draw calls outside of the swapchain.
            //! @note TODO -- having vk::VulkanRenderer or some vulkan abstracted Vulkan API specific for rendering with vulkan's API
            //! @note Drawing our tasks from command buffers -> to -> presentation images.
            static void DrawCommandBuffer(const VkCommandBuffer& cmdBuffer);

            static void DrawTriangle(const VkCommandBuffer& cmdBuffer, VulkanShaderPipelineBuilder& shaderPipeline);
        };
    }; // end of vk namespace
};
