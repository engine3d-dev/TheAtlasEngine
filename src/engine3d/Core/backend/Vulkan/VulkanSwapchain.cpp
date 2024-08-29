#include "Core/Renderer/Renderer.h"
#include <Core/EngineLogger.h>
#include <Core/backend/utilities/helper_functions.h>
#include <Core/backend/Vulkan/VulkanSwapchain.h>
#include <Core/backend/Vulkan/Vulkan.h>
#include <Core/backend/Vulkan/VulkanDevice.h>
#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>

namespace engine3d::vk{
    static VkSwapchainKHR g_Swapchain;
    static std::vector<VkImage> g_Images; // images stored in this swapchain that can be used during presentation mode
    static std::vector<VkImageView> g_ImageViews; // In vulkan we cant access directly to images, instead we need to create image views to access our images in our swapchain. Gives access to subset of our images
    static VkFormat g_SwapchainFormat;
    static VkRenderPass g_SwapchainRenderPass;
    static VkFramebuffer g_Framebuffer;
    static VkRenderPass g_RenderPass;
    static VkRenderPassBeginInfo g_RenderPassBeginInfo;

    //! @note array of framebuffers
    static std::vector<VkFramebuffer> g_Framebuffers;

    static VkRenderPassCreateInfo make_renderpass(VkDevice device, VkFormat colorFormat){
        // Render Pass
		// VkAttachmentDescription colorAttachmentDesc = {};
		// // Color attachment
		// colorAttachmentDesc.format = colorFormat;
		// colorAttachmentDesc.samples = VK_SAMPLE_COUNT_1_BIT;
		// colorAttachmentDesc.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
		// colorAttachmentDesc.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
		// colorAttachmentDesc.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
		// colorAttachmentDesc.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
		// colorAttachmentDesc.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		// colorAttachmentDesc.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

		// VkAttachmentReference colorReference = {};
		// colorReference.attachment = 0;
		// colorReference.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

		// VkAttachmentReference depthReference = {};
		// depthReference.attachment = 1;
		// depthReference.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

		// VkSubpassDescription subpassDescription = {};
		// subpassDescription.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
		// subpassDescription.colorAttachmentCount = 1;
		// subpassDescription.pColorAttachments = &colorReference;
		// subpassDescription.inputAttachmentCount = 0;
		// subpassDescription.pInputAttachments = nullptr;
		// subpassDescription.preserveAttachmentCount = 0;
		// subpassDescription.pPreserveAttachments = nullptr;
		// subpassDescription.pResolveAttachments = nullptr;

		// VkSubpassDependency dependency = {};
		// dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
		// dependency.dstSubpass = 0;
		// dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
		// dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
		// dependency.srcAccessMask = 0;
		// dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

		// VkRenderPassCreateInfo renderPassInfo = {};
		// renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
		// renderPassInfo.attachmentCount = 1;
		// renderPassInfo.pAttachments = &colorAttachmentDesc;
		// renderPassInfo.subpassCount = 1;
		// renderPassInfo.pSubpasses = &subpassDescription;
		// renderPassInfo.dependencyCount = 1;
		// renderPassInfo.pDependencies = &dependency;

        VkAttachmentDescription attachmentDescription = {
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
        
        VkAttachmentReference color_attachment_ref = {
            .attachment = 0,
            .layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL
        };

        VkSubpassDescription subpass_desc = {
            .flags = 0,
            .pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS,
            .inputAttachmentCount = 0,
            .pInputAttachments = nullptr,
            .colorAttachmentCount = 1,
            .pColorAttachments = &color_attachment_ref,
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
            .pAttachments = &attachmentDescription,
            .subpassCount = 1,
            .pSubpasses = &subpass_desc,
            .dependencyCount = 0,
            .pDependencies = nullptr
        };

        return renderPassCreateInfo;
    }

    static VkPresentModeKHR SelectPresentMode(const std::vector<VkPresentModeKHR>& presentationModes){
        for(int i = 0; i < presentationModes.size(); i++){
            if(presentationModes[i] == VK_PRESENT_MODE_MAILBOX_KHR){
                return presentationModes[i];
            }
        }

        //! @note Fallback to FIFO which is always supported
        return VK_PRESENT_MODE_FIFO_KHR;
    }

    static uint32_t SelectSizeofImages(const VkSurfaceCapabilitiesKHR& capabilities){
        uint32_t requestedImagesCount = capabilities.minImageCount + 1;

        int finalImagesCount = 0;

        if((capabilities.maxImageCount > 0) and (requestedImagesCount > capabilities.maxImageCount)){
            finalImagesCount = capabilities.maxImageCount;
        }
        else{
            finalImagesCount = requestedImagesCount;
        }

        return finalImagesCount;
    }

    //! @note Helper function to create an image view
    //! @note Wrapper helper function that includes the creation of the vkCreateImageView function!
    static VkImageView CreateImageView(VkImage img, VkFormat format, VkImageAspectFlags aspectFlags, VkImageViewType view_t, uint32_t layerCount, uint32_t mipLevels){
        g_SwapchainFormat = format;
        VkImageViewCreateInfo viewInfo = {
            .sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
            .pNext = nullptr,
            .flags = 0,
            .image = img,
            .viewType = view_t,
            .format = format,
            .components = {
                .r = VK_COMPONENT_SWIZZLE_IDENTITY,
                .g = VK_COMPONENT_SWIZZLE_IDENTITY,
                .b = VK_COMPONENT_SWIZZLE_IDENTITY,
                .a = VK_COMPONENT_SWIZZLE_IDENTITY
            },
            .subresourceRange = {
                .aspectMask = aspectFlags,
                .baseMipLevel = 0,
                .levelCount = mipLevels,
                .baseArrayLayer = 0,
                .layerCount = layerCount
            },
        };

        VkImageView imageView;
        VkResult res = vkCreateImageView(VulkanDevice::GetVkLogicalDeviceInstance(), &viewInfo, nullptr, &imageView);
        if(res != VK_SUCCESS){
            ConsoleLogError("vkCreateImageView\t\tErrored creating an image view");
        }

        return imageView;
    }

    VulkanSwapchain::~VulkanSwapchain(){
        for(int i = 0; i < g_Images.size(); i++){
            vkDestroyImageView(VulkanDevice::GetVkLogicalDeviceInstance(), g_ImageViews[i], nullptr);
        }

        vkDestroySwapchainKHR(VulkanDevice::GetVkLogicalDeviceInstance(),g_Swapchain, nullptr);
        vkDestroyDevice(VulkanDevice::GetVkLogicalDeviceInstance(),nullptr);

        //! @note TODO -- Move this in either Vulkan.h or VulkanDevice.h that handles VkSurfaceKHR
        // PFN_vkDestroySurfaceKHR vkDestroySurface = VK_NULL_HANDLE;
        // vkDestroySurface = (PFN_vkDestroySurfaceKHR)vkGetInstanceProcAddr(VulkanPipeline::GetVkInstance(), "vkDestroySurfaceKHR");

        // if(!vkDestroySurface){
        //     ConsoleLogError("Cannot find address for vkDestroyDebugUtilsMesenger!");
        //     exit(1);
        // }
    }

    void VulkanSwapchain::InitializeSwaphchain(){
        const VkSurfaceCapabilitiesKHR surfaceCap = VulkanPhysicalDevice::GetSurfaceCapabilities();
        uint32_t imagesCount = SelectSizeofImages(surfaceCap);
        
        VkPresentModeKHR presentationModes = SelectPresentMode(VulkanPhysicalDevice::GetPresentationModes());
        VkSurfaceFormatKHR surfaceFormat = SelectSurfaceFormatAndColorspace(VulkanPhysicalDevice::GetSurfaceFormats());

        VkSwapchainCreateInfoKHR swapchainCreateInfo = {
            .sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
            .pNext = nullptr,
            .flags = 0,
            .surface = VulkanPipeline::GetVkSurface(),
            .minImageCount = imagesCount,
            .imageFormat = surfaceFormat.format,
            .imageColorSpace = surfaceFormat.colorSpace,
            .imageExtent = surfaceCap.currentExtent, // currentExtent is the extent of the window
            .imageArrayLayers = 1, // relevelant to stereoscoptic surface
            .imageUsage = (VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT), // refer the usage of the images
            .imageSharingMode = VK_SHARING_MODE_EXCLUSIVE, // exclusive sharing mode just means that only one queue family can use it as a time.
            .queueFamilyIndexCount = 1,
            .pQueueFamilyIndices = &VulkanDevice::GetLogicalDevice().QueueFamilyVkCount(),
            .preTransform = surfaceCap.currentTransform,
            .compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
            .presentMode = presentationModes,
            .clipped = true,
        };

        VkResult res = vkCreateSwapchainKHR(VulkanDevice::GetVkLogicalDeviceInstance(), &swapchainCreateInfo, nullptr, &g_Swapchain);

        if(res != VK_SUCCESS){
            ConsoleLogError("vkCreateSwapchainKHR error, was unsuccessful! Error message is\t\t{}", VkResultToString(res));
        }

        uint32_t swapchainImagesCount = 0;
        res = vkGetSwapchainImagesKHR(VulkanDevice::GetVkLogicalDeviceInstance(), g_Swapchain, &swapchainImagesCount, nullptr);

        if(res != VK_SUCCESS){
            ConsoleLogError("vkGetSwapchainImagesKHR Error (1) with errored message\t\t{}", VkResultToString(res));
        }

        g_Images.resize(swapchainImagesCount);
        g_ImageViews.resize(swapchainImagesCount);

        //! @note Getting all of the handles for our images
        res = vkGetSwapchainImagesKHR(VulkanDevice::GetVkLogicalDeviceInstance(), g_Swapchain, &swapchainImagesCount, g_Images.data());

        if(res != VK_SUCCESS){
            ConsoleLogError("vkGetSwapchainImagesKHR Error (2) with errored message\t\t{}", VkResultToString(res));
        }

        int layerCount = 1;
        int mipLevels = 1;

        for(uint32_t i = 0; i < swapchainImagesCount; i++){
            g_ImageViews[i] = CreateImageView(g_Images[i], surfaceFormat.format, VK_IMAGE_ASPECT_COLOR_BIT, VK_IMAGE_VIEW_TYPE_2D, layerCount, mipLevels);
        }
        // VkRenderPassCreateInfo rpCreateInfo = make_renderpass(VulkanDevice::GetVkLogicalDeviceInstance(), SelectSurfaceFormatAndColorspace(VulkanPhysicalDevice::GetSurfaceFormats()).format);
        // VkResult rpResult = vkCreateRenderPass(VulkanDevice::GetVkLogicalDeviceInstance(), &rpCreateInfo, nullptr, &g_SwapchainRenderPass);

        // if(rpResult != VK_SUCCESS){
        //     ConsoleLogError("Swapchain vkCreateRenderPass called gave an errored message\t\t{}", VkResultToString(rpResult));
        // }

        //! @note Setting up the render pass for this swapchain
        //! @note Setting up our resources for our attachments, dependencies, and how many subpasses are there for this specific render pass.

        VkAttachmentDescription attachmentDescription = {
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

        VkAttachmentReference color_attachment_ref = {
            .attachment = 0,
            .layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL
        };

        VkSubpassDescription subpass_desc = {
            .flags = 0,
            .pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS,
            .inputAttachmentCount = 0,
            .pInputAttachments = nullptr,
            .colorAttachmentCount = 1,
            .pColorAttachments = &color_attachment_ref,
            .pResolveAttachments = nullptr,
            .pDepthStencilAttachment = nullptr,
            .preserveAttachmentCount = 0,
            .pPreserveAttachments = nullptr
        };
        VkRenderPassCreateInfo rpBeginInfo = {
            .sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO,
            .pNext = nullptr,
            .flags = 0,
            .attachmentCount = 1,
            .pAttachments = &attachmentDescription,
            .subpassCount = 1,
            .pSubpasses = &subpass_desc,
            .dependencyCount = 0,
            .pDependencies = nullptr
        };

        VkResult rpResult = vkCreateRenderPass(VulkanDevice::GetVkLogicalDeviceInstance(), &rpBeginInfo, nullptr, &g_RenderPass);

        //! @note Initiating renderpass begin info
        g_RenderPassBeginInfo = {
            .sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO,
            .pNext = nullptr,
            .renderPass = g_RenderPass,
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

        //! @note Initiating our framebuffers
        g_Framebuffers.resize(VulkanSwapchain::GetImagesSize());
        for(uint32_t i = 0; i < VulkanSwapchain::GetImagesSize(); i++){
            VkFramebufferCreateInfo fbCreateInfo = {
                .sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO,
                .renderPass = g_RenderPass,
                .attachmentCount = 1,
                .pAttachments = &VulkanSwapchain::GetImageView(i),
                .width = VulkanPipeline::GetWidth(),
                .height = VulkanPipeline::GetHeight(),
                .layers = 1
            };

            VkResult fbRes = vkCreateFramebuffer(VulkanDevice::GetVkLogicalDeviceInstance(), &fbCreateInfo, nullptr, &g_Framebuffers[i]);

            if(fbRes != VK_SUCCESS){
                ConsoleLogError("Framebuffer at index ({}) errored out! Error message is\t\t{}", i, VkResultToString(fbRes));
            }
        }

    }

    VkSwapchainKHR& VulkanSwapchain::GetVkSwapchainInstance(){
        return g_Swapchain;
    }

    VkImage& VulkanSwapchain::GetImage(uint32_t index){
        return g_Images[index];
    }

    VkImageView& VulkanSwapchain::GetImageView(uint32_t index){
        return g_ImageViews[index];
    }

    uint32_t VulkanSwapchain::GetImagesSize() { return g_Images.size(); }

    VkFormat& VulkanSwapchain::GetSwapchainFormat() { return g_SwapchainFormat; }
    
    VkRenderPass& VulkanSwapchain::GetSwapchainRenderPass() { return g_SwapchainRenderPass; }

    void VulkanSwapchain::DrawCommandBuffer(const VkCommandBuffer& cmdBuffer){
        vk::Begin(cmdBuffer, VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT);
        vkCmdBeginRenderPass(cmdBuffer, &g_RenderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);

        vkCmdEndRenderPass(cmdBuffer);
        vk::End(cmdBuffer);
    }

    void VulkanSwapchain::DrawTriangle(const VkCommandBuffer& cmdBuffer, VulkanShaderPipelineBuilder& shaderPipeline){
        vk::Begin(cmdBuffer, VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT);
        vkCmdBeginRenderPass(cmdBuffer, &g_RenderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);
        

        //! @note Draw stuff in between here!
        vkCmdBindPipeline(cmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, shaderPipeline.GetVkPipeline());
        vkCmdDraw(cmdBuffer, 3, 1, 0, 0);

        vkCmdEndRenderPass(cmdBuffer);
        vk::End(cmdBuffer);
    }

    void VulkanSwapchain::SetCurrentFramebuffer(uint32_t index){
        g_RenderPassBeginInfo.framebuffer = g_Framebuffers[index];
    }

    VkFramebuffer& VulkanSwapchain::ReadFramebuffer(uint32_t FrameIndex){
        return g_Framebuffers[FrameIndex];
    }

};
