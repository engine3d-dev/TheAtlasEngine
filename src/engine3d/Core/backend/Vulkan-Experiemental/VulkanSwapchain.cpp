#include <Core/EngineLogger.h>
#include <Core/backend/utilities/helper_functions.h>
#include <Core/backend/Vulkan-Experiemental/VulkanSwapchain.h>
#include <Core/backend/Vulkan-Experiemental/Vulkan.h>
#include <Core/backend/Vulkan-Experiemental/VulkanDevice.h>
#include <Vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>

namespace engine3d::vk{
    static VkSwapchainKHR g_Swapchain;
    static std::vector<VkImage> g_Images; // images stored in this swapchain that can be used during presentation mode
    static std::vector<VkImageView> g_ImageViews; // In vulkan we cant access directly to images, instead we need to create image views to access our images in our swapchain. Gives access to subset of our images

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
            .pQueueFamilyIndices = &VulkanDevice::GetLogicalDevice().QueueFamily(),
            .preTransform = surfaceCap.currentTransform,
            .compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
            .presentMode = presentationModes,
            .clipped = true,
        };

        VkResult res = vkCreateSwapchainKHR(VulkanDevice::GetVkLogicalDeviceInstance(), &swapchainCreateInfo, nullptr, &g_Swapchain);

        if(res != VK_SUCCESS){
            ConsoleLogError("vkCreateSwapchainKHR error, was unsuccessful! Error message is\t\t{}", VkResultToString(res));
        }
        // else{
        //     ConsoleLogInfo("vkCreateSwapchainKHR was created successful!");
        // }

        uint32_t swapchainImagesCount = 0;
        res = vkGetSwapchainImagesKHR(VulkanDevice::GetVkLogicalDeviceInstance(), g_Swapchain, &swapchainImagesCount, nullptr);

        if(res != VK_SUCCESS){
            ConsoleLogError("vkGetSwapchainImagesKHR Error (1) with errored message\t\t{}", VkResultToString(res));
        }
        // else{
        //     ConsoleLogInfo("Swapchain\t\tNumber of Images {}", swapchainImagesCount);
        // }

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

};