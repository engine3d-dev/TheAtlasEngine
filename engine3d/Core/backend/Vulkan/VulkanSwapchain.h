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
            ~VulkanSwapchain();

            //! @note Initiating our vulkan swapchain.
            void InitializeSwaphchain();

            //! @note Getting our image/image view from our swapchain
            static uint32_t GetImagesSize();
            static VkImage& GetImage(uint32_t index);
            static VkImageView& GetImageView(uint32_t index);

            //! @note Fetch the swapchain handler
            static VkSwapchainKHR& GetVkSwapchainInstance();
        };
    }; // end of vk namespace
};