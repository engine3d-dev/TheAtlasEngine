#include <drivers/swapchain.hpp>
#include <core/application_instance.hpp>
#include <drivers/vulkan/vulkan_context.hpp>
#include <drivers/vulkan/vulkan_swapchain.hpp>

namespace engine3d{
    Ref<Swapchain> Swapchain::InitializeSwapchain(VkSurfaceKHR p_Surface){
        switch (ApplicationInstance::CurrentAPI()){
        case API::VULKAN: return CreateRef<vk::VulkanSwapchain>(vk::VulkanContext::GetPhysicalDriver(), vk::VulkanContext::GetDriver(), p_Surface);
        default:
            break;
        }
        return nullptr;
    }

    void Swapchain::OnResize(uint32_t Width, uint32_t Height){
        RecreateSwapchain(Width, Height);
    }

    uint32_t Swapchain::GetImagesSize() const{
        return ImagesSize();
    }

    bool Swapchain::IsResized(){
        return SwapchainResized();
    }

    void Swapchain::SetResizeStatus(bool p_IsResizeSignaled){
        ResizeStatus(p_IsResizeSignaled);
    }

    VkFormat& Swapchain::GetSwapchainFormat(){
        return ReadSwapchainFormat();
    }

    VkFramebuffer Swapchain::GetFramebuffer(uint32_t index){
        return ReadFramebuffer(index);
    }

    VkRenderPass Swapchain::GetRenderPass(){
        return ReadSwapchainRenderPass();
    }

    VkImageView Swapchain::GetImageView(uint32_t index){
        return ReadImageView(index);
    }

    VkSwapchainKHR Swapchain::GetVkSwapchainHandler(){
        return VkSwapchainHandler();
    }

    VkExtent2D Swapchain::GetSwapchainExtent(){
        return ReadSwapchainExtent();
    }

    void Swapchain::SubmitCommandBuffer(VkCommandBuffer* p_CommandBuffers){
        return SubmitAndWriteCommandBuffer(p_CommandBuffers);
    }

    //! @note Proceeds to getting next frame.
    uint32_t Swapchain::AcquireNextImage(){
        return ReadAcquiredNextFrame();
    }


    //! @note m_CurrentFrameIdx
    uint32_t Swapchain::GetCurrentFramePerTick(){
        return CurrentFramePerTick();
    }
};