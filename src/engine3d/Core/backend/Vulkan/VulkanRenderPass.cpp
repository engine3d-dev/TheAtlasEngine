#include <Core/EngineLogger.h>
#include <Core/backend/Vulkan/Vulkan.h>
#include <Core/backend/Vulkan/VulkanSwapchain.h>
#include <Core/backend/Vulkan/VulkanDevice.h>
#include <Core/backend/Vulkan/VulkanRenderPass.h>
#include <Core/backend/utilities/helper_functions.h>
#include <vulkan/vulkan_core.h>
#include <Core/Renderer/Renderer.h>

namespace engine3d::vk{

    struct VulkanRenderPassDescriptors{
        VkRenderPassBeginInfo renderPassBeginInfo;
        VkAttachmentDescription attachmentDescription;
        VkAttachmentReference color_attachment_ref;
    };

    static VulkanRenderPassDescriptors g_Descriptors;

    VulkanRenderPass::VulkanRenderPass(const std::string& debugName){

        g_Descriptors.attachmentDescription = {
            .flags = 0,
            //! @note render pass uses this format
            //! @note attachment will have format req by swapchain
            .format = SelectSurfaceFormatAndColorspace(VulkanPhysicalDevice::GetSurfaceFormats()).format,
            //! @note sample 1-bit, wont be using MSAA
            .samples = VK_SAMPLE_COUNT_1_BIT, // used for multisampling
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
        
        g_Descriptors.color_attachment_ref = {
            .attachment = 0,
            .layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL
        };

        VkSubpassDescription subpass_desc = {
            .flags = 0,
            .pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS,
            .inputAttachmentCount = 0,
            .pInputAttachments = nullptr,
            .colorAttachmentCount = 1,
            .pColorAttachments = &g_Descriptors.color_attachment_ref,
            .pResolveAttachments = nullptr,
            .pDepthStencilAttachment = nullptr,
            .preserveAttachmentCount = 0,
            .pPreserveAttachments = nullptr
        };

        //! @note Setting up our resources for our attachments, dependencies, and how many subpasses are there for this specific render pass.
        VkRenderPassCreateInfo renderPassCreateInfo = {
            .sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO,
            .pNext = nullptr,
            .flags = 0,
            .attachmentCount = 1,
            .pAttachments = &g_Descriptors.attachmentDescription,
            .subpassCount = 1,
            .pSubpasses = &subpass_desc,
            .dependencyCount = 0,
            .pDependencies = nullptr
        };

        VkResult res = vkCreateRenderPass(VulkanDevice::GetVkLogicalDeviceInstance(), &renderPassCreateInfo, nullptr, &m_RenderPass);

        if(res != VK_SUCCESS){
            ConsoleLogError("vkCreateRenderPass errored out! Error message is\t\t{}", VkResultToString(res));
        }

        //! @note RenderpassesBeginInfo is like equivalent to setting up configurations of the this specific render pass
        //! @note Meaning instead of setting the clear color and recording it. This descriptor begin info already records our clear color.
        g_Descriptors.renderPassBeginInfo = {
            .sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO,
            .pNext = nullptr,
            .renderPass = m_RenderPass,
            .renderArea = {
                .offset = {
                    .x = 0,
                    .y = 0
                },
                .extent = {
                    .width = VulkanPipeline::GetWidth(),
                    .height = VulkanPipeline::GetHeight()
                },
            },
            .clearValueCount = 1,
            .pClearValues = Renderer::GetClearColor() // This is how we set our color to our window without actually doing our recording.
        };

        ConsoleLogInfo("VulkanRenderPass Initiated!");
        ConsoleLogInfo("VulkanRendererPass Debug Name === {}", debugName);

        //! @note Initializing framebuffers
        m_Framebuffers.resize(VulkanSwapchain::GetImagesSize());

        for(uint32_t i = 0; i < VulkanSwapchain::GetImagesSize(); i++){
            VkFramebufferCreateInfo fbCreateInfo = {
                .sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO,
                .renderPass = m_RenderPass,
                .attachmentCount = 1,
                .pAttachments = &VulkanSwapchain::GetImageView(i),
                .width = VulkanPipeline::GetWidth(),
                .height = VulkanPipeline::GetHeight(),
                .layers = 1
            };

            VkResult fbRes = vkCreateFramebuffer(VulkanDevice::GetVkLogicalDeviceInstance(), &fbCreateInfo, nullptr, &m_Framebuffers[i]);

            if(fbRes != VK_SUCCESS){
                ConsoleLogError("Framebuffer at index ({}) errored out! Error message is\t\t{}", i, VkResultToString(fbRes));
            }
        }

        ConsoleLogInfo("Framebuffers also created after render pass!");
    }

    void VulkanRenderPass::Begin(VkCommandBuffer buffer, VkSubpassContents contents){
        if(m_RenderPass == nullptr){
            ConsoleLogError("This could mean m_RenderPass was in fact nullptr!");
        }
        vkCmdBeginRenderPass(buffer, &g_Descriptors.renderPassBeginInfo, contents);
    }

    void VulkanRenderPass::End(VkCommandBuffer buffer){
        if(m_RenderPass == nullptr){
            ConsoleLogError("This could mean m_RenderPass was in fact nullptr!");
        }
        vkCmdEndRenderPass(buffer);
    }

    void VulkanRenderPass::SetCurrentFramebuffer(uint32_t idx){
        g_Descriptors.renderPassBeginInfo.framebuffer = m_Framebuffers[idx];
    }

    VkRenderPass& VulkanRenderPass::GetVkRenderPass(){ return m_RenderPass; }

};