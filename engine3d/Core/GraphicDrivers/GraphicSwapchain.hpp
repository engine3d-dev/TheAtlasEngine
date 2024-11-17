#pragma once

#include "Core.hpp"
#include <vulkan/vulkan_core.h>
namespace engine3d{
    class GraphicSwapchain{
    public:
        //! @note Initialize our swapchain
        Ref<GraphicSwapchain> InitializeSwapchain();

        //! @note Public API's
        VkSwapchainKHR GetVkSwapchainHandler();
        
    };
};