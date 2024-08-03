#pragma once
#include <vulkan/vulkan.h>
#include <vector>

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
        */
        class VulkanSwapchain{
        public:
            // explicit VulkanSwapchain();
            ~VulkanSwapchain();
            void InitializeSwaphchain();

        private:
            VkSwapchainKHR m_Swapchain;
            std::vector<VkImage> m_Images; // images stored in this swapchain that can be used during presentation mode
            std::vector<VkImageView> m_ImageViews; // In vulkan we cant access directly to images, instead we need to create image views to access our images in our swapchain. Gives access to subset of our images
        };
    }; // end of vk namespace
};