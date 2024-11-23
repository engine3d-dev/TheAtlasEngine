#include "ApplicationInstance.hpp"
#include "internal/Vulkan2Showcase/VulkanSwapchain.hpp"
#include "internal/Vulkan2Showcase/VulkanContext.hpp"
#include <Core/GraphicDrivers/GraphicSwapchain.hpp>
#include <vulkan/vulkan_core.h>

namespace engine3d{
    Ref<GraphicSwapchain> GraphicSwapchain::InitializeSwapchain(VkSurfaceKHR p_Surface){
        switch (ApplicationInstance::CurrentAPI()){
        case API::VULKAN: return CreateRef<vk::VulkanSwapchain>(vk::VulkanContext::GetPhysicalDriver(), vk::VulkanContext::GetDriver(), p_Surface);
        default:
            break;
        }
        return nullptr;
    }

    uint32_t GraphicSwapchain::GetImagesSize() const{
        return ImagesSize();
    }

    VkFormat GraphicSwapchain::GetSwapchainFormat(){
        return ReadSwapchainFormat();
    }

    VkFramebuffer GraphicSwapchain::GetFramebuffer(uint32_t index){
        return ReadFramebuffer(index);
    }

    VkRenderPass GraphicSwapchain::GetRenderPass(){
        return ReadSwapchainRenderPass();
    }

    VkImageView GraphicSwapchain::GetImageView(uint32_t index){
        return ReadImageView(index);
    }

    VkSwapchainKHR GraphicSwapchain::GetVkSwapchainHandler(){
        return VkSwapchainHandler();
    }

    VkExtent2D GraphicSwapchain::GetSwapchainExtent(){
        return ReadSwapchainExtent();
    }

    void GraphicSwapchain::SubmitCommandBuffer(VkCommandBuffer* p_CommandBuffers){
        return SubmitAndWriteCommandBuffer(p_CommandBuffers);
    }

    //! @note Proceeds to getting next frame.
    uint32_t GraphicSwapchain::AcquireNextImage(){
        return ReadAcquiredNextFrame();
    }


    //! @note m_CurrentFrameIdx
    uint32_t GraphicSwapchain::GetCurrentFramePerTick(){
        return CurrentFramePerTick();
    }
};