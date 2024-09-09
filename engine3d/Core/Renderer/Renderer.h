#pragma once

#include <array>
#include <vulkan/vulkan.h>

namespace engine3d{
    /**
     * @name Renderer
     * @note This will be Engine3D's renderer abstracted away the Vulkan abstraction
     * @note Renderer's role within Engine3D's core is to have an API that can be used across to submit tasks
     * @note When to make render passes per our renderer draw call.
     * @note When users want to render primitives such as via Renderer::DrawCube(...) per this draw call will submit this into a queue before submission.
     * @note Core renderer for Engine3D.
     * @note NOTE -- May have Renderer2D to enable rendering in 2-dimensions.
     * @note While Renderer will be used to do real-time (RT) rendering and 3D rendering
    */
    class Renderer{
    public:
    
        //! @note This will be how our renderer get's initialized
        static void Initialize();
        //! @note Per indices will represent our rgba values
        /**
         * @param 0 =   r
         * @param 1 =   g
         * @param 2 =   b
         * @param 3 =   a
        */
        static void SetBackgroundColor(const std::array<float, 4>& rgba);

        //! @note Will want a function to flush our scene when giving the renderer all of our context
        static void FlushScene();

        static VkClearValue* GetClearColor();

        template<typename UTask>
        static void Submit(const UTask&& task){
        }

        //! @note This is what will be used to submit our color when refreshing frames.
        // static void RenderClearColor();
        //! @note Testing rendering basic clear frames
        static void RenderBasicClearColor();

        static void Presentation();

    private:
    };
};

/**
 * 
 * 
 * 
 * 
 * 

!@note Changing command buffers API


//! @note This is equivalent to doing:
vk::Begin(m_CommandBuffer);
// Do the actual task below here
vk::End(m_CommandBuffer);

//! @note Preferred API. Since recording tasks into command buffer may vary.
//! @note Option #1
m_ClearColorCommandBuffer.Record([](){
    VkImageSubresourceRange imageRange = {
        .aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT,
        .baseMipLevel = 0,
        .levelCount = 1,
        .baseArrayLayer = 0,
        .layerCount = 1
    };

    uint32_t presentQueueFamily = vk::VulkanDevice::GetLogicalDevice().QueueFamily();
    
    vkCmdClearColorImage(g_ClearColorCommandBuffer, vk::VulkanSwapchain::GetImage(0), VK_IMAGE_LAYOUT_GENERAL, &clearColorValue.color, 1, &imageRange);
});

//! @note Option #2

vk::Record(m_CommandBuffer, [](){
    VkImageSubresourceRange imageRange = {
        .aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT,
        .baseMipLevel = 0,
        .levelCount = 1,
        .baseArrayLayer = 0,
        .layerCount = 1
    };

    uint32_t presentQueueFamily = vk::VulkanDevice::GetLogicalDevice().QueueFamily();
    
    vkCmdClearColorImage(g_ClearColorCommandBuffer, vk::VulkanSwapchain::GetImage(0), VK_IMAGE_LAYOUT_GENERAL, &clearColorValue.color, 1, &imageRange);
});
*/