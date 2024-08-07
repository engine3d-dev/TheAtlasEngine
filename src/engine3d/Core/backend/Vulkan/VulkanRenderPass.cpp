#include <Core/EngineLogger.h>
#include <Core/backend/Vulkan/Vulkan.h>
#include <Core/backend/Vulkan/VulkanSwapchain.h>
#include <Core/backend/Vulkan/VulkanDevice.h>
#include <Core/backend/Vulkan/VulkanRenderPass.h>
#include <Core/backend/utilities/helper_functions.h>
#include <vulkan/vulkan_core.h>

namespace engine3d::vk{

    void VulkanRenderPass::InitializeRenderPass(){
        //! @note Attachment Description is description of the image being written into with rendering commands.
        VkAttachmentDescription color_attachment = {
            //! @note render pass uses this format
            //! @note attachment will have format req by swapchain
            .format = SelectSurfaceFormatAndColorspace(VulkanPhysicalDevice::GetSurfaceFormats()).format,
            //! @note sample 1-bit, wont be using MSAA
            .samples = VK_SAMPLE_COUNT_1_BIT,
            //! @note clear when attachment's loaded
            .loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR,
            //! @note keeping attachmentt stored when render pass ends
            .storeOp = VK_ATTACHMENT_STORE_OP_STORE,
            //! @note not going to be doing stencil for rn.
            .stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE,
            .stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE,
            //! @note not using initial layout of attachment for rn
            .initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
            //! @note after render pass ends, image has to be on layour ready to be displayed.
            .finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR
        };

        VkAttachmentReference color_attach_ref = {
            .attachment = 0,
            .layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL
        };

        VkSubpassDescription subpass = {
            .pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS,
            .colorAttachmentCount = 1,
            .pColorAttachments = &color_attach_ref
        };

        VkRenderPassCreateInfo render_pass_info = {
            .sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO,
            .attachmentCount = 1,
            .pAttachments = &color_attachment,
            .subpassCount = 1,
            .pSubpasses = &subpass
        };

        // m_RenderPassCreateInfo = render_pass_info;

        VkResult res = vkCreateRenderPass(VulkanDevice::GetVkLogicalDeviceInstance(), &render_pass_info, nullptr, &m_RenderPass);

        if(res != VK_SUCCESS){
            ConsoleLogError("vkCreateRenderPass errored in VulkanRenderPass.cpp with error message\t\t{}", VkResultToString(res));
            std::terminate();
        }

        ConsoleLogInfo("Render Pass Created!");
    }

    void VulkanRenderPass::InitializeFramebuffers(){
        VkFramebufferCreateInfo fb_create_info = {
            .sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO,
            .pNext = nullptr,
            .renderPass = m_RenderPass,
            .attachmentCount = 1,
            .width = VulkanPipeline::GetWidth(),
            .height = VulkanPipeline::GetHeight(),
            .layers = 1
        };

        //! @note Indicating to vulkan how many images we have in the swapchain
        const uint32_t swapchain_imagecount = VulkanSwapchain::GetImagesSize();
        m_Framebuffers.resize(swapchain_imagecount);

        //! @note Creating framebuffer for each swapchain image views
        for(uint32_t i = 0; i < swapchain_imagecount; i++){
            fb_create_info.pAttachments = &VulkanSwapchain::GetImageView(i);
            VkResult res = vkCreateFramebuffer(VulkanDevice::GetVkLogicalDeviceInstance(), &fb_create_info, nullptr, &m_Framebuffers[i]);
            
            if(res != VK_SUCCESS){
                ConsoleLogError("vkCreateFramebuffer errored at index {} with error message\t\t{}", i, VkResultToString(res));
                std::terminate();
            }
        }

        ConsoleLogInfo("VkFramebuffer Created!");
    }

    void VulkanRenderPass::BeginPass(uint32_t swapchainIdx, VkCommandBuffer command, VkSubpassContents flags, VkPipeline pipeline){
        //make a clear-color from frame number. This will flash with a 120*pi frame period.
        VkClearValue clearValue;
        // float flash = abs(sin(_frameNumber / 120.f));
        clearValue.color = { { 0.0f, 0.5f, 0.5f, 1.0f } };
        VkRenderPassBeginInfo renderPassBeginInfo = {
            .sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO,
            .pNext = nullptr,
            .renderPass = m_RenderPass,
            .renderArea.offset.x = 0,
            .renderArea.offset.y = 0,
            .renderArea.extent = {VulkanPipeline::GetWidth(), VulkanPipeline::GetHeight()},
            .framebuffer = m_Framebuffers[swapchainIdx],
            .clearValueCount = 1,
            .pClearValues = &clearValue
        };

        vkCmdBeginRenderPass(command, &renderPassBeginInfo, flags);
    }

    void VulkanRenderPass::EndPass(VkCommandBuffer command){
        vkCmdEndRenderPass(command);
    }

    VkRenderPass VulkanRenderPass::GetRenderPassInstnace(){ return m_RenderPass; }
};