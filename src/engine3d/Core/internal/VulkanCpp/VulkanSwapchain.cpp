#include "EngineLogger.h"
#include "Renderer/Renderer.hpp"
#include "internal/VulkanCpp/Vulkan.h"
#include "internal/VulkanCpp/VulkanDevice.h"
#include <Core/internal/VulkanCpp/VulkanSwapchain.h>

#include <stdexcept>


#if _WIN32
#define VK_USE_PLATFORM_WIN32_KHR
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>
#include <vulkan/vulkan_win32.h>
#include <vulkan/vulkan_core.h>
#define NOMINMAX
#include <limits>
#else
#include <GLFW/glfw3.h>
#include <vulkan/vulkan.h>
#endif


static PFN_vkGetPhysicalDeviceSurfaceSupportKHR fpGetPhysicalDeviceSurfaceSupportKHR;
static PFN_vkGetPhysicalDeviceSurfaceCapabilitiesKHR fpGetPhysicalDeviceSurfaceCapabilitiesKHR;


namespace engine3d::vk{
    VulkanSwapchain::VulkanSwapchain(GLFWwindow* p_WindowHandler, bool p_VSync){
        m_VSync = p_VSync;

        glfwCreateWindowSurface(Vulkan::GetVkInstance(), p_WindowHandler, nullptr, &m_Surface);

        auto physical_device = VulkanDevice::GetPhysicalDevice().GetVkPhysicalDevice();
        uint32_t queue_count = 0;
        // vkGetPhysicalDeviceQueueFamilyProperties(physical_device, &queue_count, nullptr);
        // assert(queue_count >= 1);
        // if(queue_count >= 1){
        //     ConsoleLogWarn("Queue Count >= 1");
        // }

        // ConsoleLogError("queue_count = {0}", queue_count);
        // std::vector<VkQueueFamilyProperties> queue_properties(queue_count);
        // vkGetPhysicalDeviceQueueFamilyProperties(physical_device, &queue_count, queue_properties.data());

        //! @note Using our physical device to check what presentation mode is available on our current platform.
        // std::vector<VkBool32> presentation_supported(queue_count);
        // for(uint32_t i = 0; i < queue_count; i++){
        //     vkGetPhysicalDeviceSurfaceSupportKHR(physical_device, i, m_Surface, &presentation_supported[i]);
        // }
        
        // // uint32_t graphics_queue_node_idx = std::numeric_limits<size_t>::max();
        // uint32_t graphics_queue_node_idx = UINT32_MAX;
        // uint32_t presentation_queue_node_idx = UINT32_MAX;

        // //! @note We fetch the index of our queue type.
        // for(uint32_t i = 0; i < queue_count; i++){
        //     if((queue_properties[i].queueFlags & VK_QUEUE_GRAPHICS_BIT)){
        //         if(graphics_queue_node_idx == UINT32_MAX){
        //             graphics_queue_node_idx = i;
        //         }

        //         if(presentation_supported[i]){
        //             graphics_queue_node_idx = i;
        //             presentation_queue_node_idx = i;
        //             break;
        //         }
        //     }
        // }

        // if(presentation_queue_node_idx == UINT32_MAX){
        //     for(uint32_t i = 0; i < queue_count; i++){
        //         if(presentation_supported[i]){
        //             presentation_queue_node_idx = i;
        //             break;
        //         }
        //     }
        // }

        // if(graphics_queue_node_idx == UINT32_MAX){
        //     ConsoleLogError("graphics_queue_node_idx == UINT32_MAX");
        // }

        // if(presentation_queue_node_idx == UINT32_MAX){
        //     ConsoleLogError("presentation_queue_node_idx == UINT32_MAX");
        // }

        // m_queue_node_idx = graphics_queue_node_idx;

        // SearchImageFormatAndColorspace();

        // //! @note Initiating Swapchain
        // auto device = VulkanDevice::GetLogicalDevice().GetVkDeviceInstance();
        // // auto physical_device = VulkanDevice::GetPhysicalDevice().GetVkPhysicalDevice();

        // //! @note Extracting presentation modes available.
        // uint32_t presentation_mode_count;
        // vkGetPhysicalDeviceSurfacePresentModesKHR(physical_device, m_Surface, &presentation_mode_count, NULL);
        // std::vector<VkPresentModeKHR> presentModes(presentation_mode_count);
		// vkGetPhysicalDeviceSurfacePresentModesKHR(physical_device, m_Surface, &presentation_mode_count, presentModes.data());

        // VkSurfaceCapabilitiesKHR surface_capabilities;
        // vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physical_device, m_Surface, &surface_capabilities);

        // VkExtent2D swapchain_extent = {};

        // if(surface_capabilities.currentExtent.width == (uint32_t)-1){
        //     // If the surface capabilities are -1 or not set we set them.
        //     int width, height;
        //     glfwGetWindowSize(p_WindowHandler, &width, &height);
        //     swapchain_extent.width = width;
        //     swapchain_extent.height = height;
        // }
        // else{
        //     swapchain_extent = surface_capabilities.currentExtent;
        //     m_Width = surface_capabilities.currentExtent.width;
        //     m_Height = surface_capabilities.currentExtent.height;
        // }

        // if(m_Width == 0 || m_Height == 0){
        //     return;
        // }

        // VkPresentModeKHR swapchain_present_mode = VK_PRESENT_MODE_FIFO_KHR;

        // // If v-sync is not requested, try to find a mailbox mode
		// // It's the lowest latency non-tearing present mode available
        // if(!m_VSync){
        //     for (size_t i = 0; i < presentation_mode_count; i++){
		// 		if (presentModes[i] == VK_PRESENT_MODE_MAILBOX_KHR){
		// 			swapchain_present_mode = VK_PRESENT_MODE_MAILBOX_KHR;
		// 			break;
		// 		}
		// 		if ((swapchain_present_mode != VK_PRESENT_MODE_MAILBOX_KHR) && (presentModes[i] == VK_PRESENT_MODE_IMMEDIATE_KHR)){
		// 			swapchain_present_mode = VK_PRESENT_MODE_IMMEDIATE_KHR;
		// 		}
		// 	}
        // }


        // //! @note Determining number of images.
        // uint32_t desired_number_of_images = surface_capabilities.minImageCount + 1;

        // if((surface_capabilities.maxImageCount > 0) and (desired_number_of_images > surface_capabilities.maxImageCount)){
        //     desired_number_of_images = surface_capabilities.maxImageCount;
        // }

        // // Searching transformation of our surface.
        // VkSurfaceTransformFlagBitsKHR pres_transform;
        // if(surface_capabilities.supportedTransforms & VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR){
		// 	// We prefer a non-rotated transform
		// 	pres_transform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
		// }
		// else{
		// 	pres_transform = surface_capabilities.currentTransform;
		// }

        // // Find a supported composite alpha format (not all devices support alpha opaque)
		// VkCompositeAlphaFlagBitsKHR composite_alpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
		// // Simply select the first composite alpha format available
		// std::vector<VkCompositeAlphaFlagBitsKHR> compositeAlphaFlags = {
		// 	VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
		// 	VK_COMPOSITE_ALPHA_PRE_MULTIPLIED_BIT_KHR,
		// 	VK_COMPOSITE_ALPHA_POST_MULTIPLIED_BIT_KHR,
		// 	VK_COMPOSITE_ALPHA_INHERIT_BIT_KHR,
		// };
		// for (auto& compositeAlphaFlag : compositeAlphaFlags) {
		// 	if (surface_capabilities.supportedCompositeAlpha & compositeAlphaFlag) {
		// 		composite_alpha = compositeAlphaFlag;
		// 		break;
		// 	};
		// }

        // VkSwapchainCreateInfoKHR createInfo = {
        //     .sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
        //     .pNext = nullptr,
        //     .surface = m_Surface,
        //     .minImageCount = desired_number_of_images,
        //     .imageFormat = m_ColorFormat,
        //     .imageColorSpace = m_ColorSpace,
        //     .imageExtent = {swapchain_extent.width, swapchain_extent.height},
        //     .imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
        //     .preTransform = (VkSurfaceTransformFlagBitsKHR)pres_transform,
        //     .imageArrayLayers = 1,
        //     .imageSharingMode = VK_SHARING_MODE_EXCLUSIVE,
        //     .queueFamilyIndexCount = 0,
        //     .pQueueFamilyIndices = nullptr,
        //     .presentMode = swapchain_present_mode,
        //     .oldSwapchain = m_OldSwapchain,
        //     .clipped = true,
        //     .compositeAlpha = composite_alpha
        // };

        // vkCreateSwapchainKHR(device, &createInfo, nullptr, &m_CurrentSwapchain);

        // if(m_OldSwapchain){
        //     vkDestroySwapchainKHR(device, m_OldSwapchain, nullptr);
        // }



        // //! @note Determining images.
        // uint32_t images_size = 0;
        // for(auto& image : m_SwapchainImages){
        //     vkDestroyImageView(device, image.image_view, nullptr);
        // }
        // m_Images.clear();

        // vkGetSwapchainImagesKHR(device, m_CurrentSwapchain, &images_size, nullptr);
        // m_Images.resize(images_size);
        // m_Images.resize(images_size);
        // vkGetSwapchainImagesKHR(device, m_CurrentSwapchain, &images_size, m_Images.data());

        // for(uint32_t i = 0; i < m_Images.size(); i++){
        //     VkImageViewCreateInfo imageViewCreateInfo = {
        //         .sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
        //         .pNext = nullptr,
        //         .format = m_ColorFormat,
        //         .image = m_Images[i],
        //         .components = {
        //             VK_COMPONENT_SWIZZLE_G,
        //             VK_COMPONENT_SWIZZLE_B,
        //             VK_COMPONENT_SWIZZLE_A
        //         },
        //         .subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
        //         .subresourceRange.baseMipLevel = 0,
        //         .subresourceRange.levelCount = 1,
        //         .subresourceRange.baseArrayLayer = 0,
        //         .subresourceRange.layerCount = 1,
        //         .viewType = VK_IMAGE_VIEW_TYPE_2D,
        //         .flags = 0
        //     };

        //     m_SwapchainImages[i].image = m_Images[i];
            
        //     vkCreateImageView(device, &imageViewCreateInfo, nullptr, &m_SwapchainImages[i].image_view);
        // }

        // //! @note Swapchain Creating its own sets of command buffers
        // for(auto& command : m_CommandBuffers){
        //     vkDestroyCommandPool(device, command.CommandBufferPool, nullptr);
        // }

        // VkCommandPoolCreateInfo command_pool_create_info = {
        //     .sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
        //     .queueFamilyIndex = m_queue_node_idx,
        //     .flags = VK_COMMAND_POOL_CREATE_TRANSIENT_BIT,
        // };

        // VkCommandBufferAllocateInfo allocateInfo = {
        //     .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
		// 	.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
		// 	.commandBufferCount = 1
        // };

        // m_CommandBuffers.resize(images_size);
        // for(auto& command_buffer : m_CommandBuffers){
        //     vkCreateCommandPool(device, &command_pool_create_info, nullptr, &command_buffer.CommandBufferPool);
        //     vkAllocateCommandBuffers(device, &allocateInfo, &command_buffer.CommndBufferData);
        // }

        // auto frames_in_flight = Renderer::GetSettings().FramesInFlight;
        // if(m_ImageAvailableSemaphores.size() != frames_in_flight){
        //     m_ImageAvailableSemaphores.resize(frames_in_flight);
		// 	// m_RenderFinishedSemaphores.resize(frames_in_flight);
        //     m_ImagesFinishedRenderingSemaphores.resize(frames_in_flight);
		// 	VkSemaphoreCreateInfo semaphoreCreateInfo{};
		// 	semaphoreCreateInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
		// 	for (size_t i = 0; i < frames_in_flight; i++){
		// 		vkCreateSemaphore(device, &semaphoreCreateInfo, nullptr, &m_ImageAvailableSemaphores[i]);
		// 		// VKUtils::SetDebugUtilsObjectName(device, VK_OBJECT_TYPE_SEMAPHORE, std::format("Swapchain Semaphore ImageAvailable {0}", i), m_ImageAvailableSemaphores[i]);
		// 		vkCreateSemaphore(device, &semaphoreCreateInfo, nullptr, &m_ImagesFinishedRenderingSemaphores[i]);
		// 		// VKUtils::SetDebugUtilsObjectName(device, VK_OBJECT_TYPE_SEMAPHORE, std::format("Swapchain Semaphore RenderFinished {0}", i), m_RenderFinishedSemaphores[i]);
		// 	}
        // }

        // if(m_Fences.size() != frames_in_flight){
		// 	VkFenceCreateInfo fenceCreateInfo{};
		// 	fenceCreateInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
		// 	fenceCreateInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

		// 	m_Fences.resize(frames_in_flight);
		// 	for (auto& fence : m_Fences){
		// 		vkCreateFence(device, &fenceCreateInfo, nullptr, &fence);
		// 		// VKUtils::SetDebugUtilsObjectName(device, VK_OBJECT_TYPE_FENCE, "Swapchain Fence", fence);
		// 	}
		// }

        // VkPipelineStageFlags pipelineStageFlags = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;

		// VkFormat depthFormat = VulkanDevice::GetPhysicalDevice().GetDepthFormat();

		// // Render Pass
		// VkAttachmentDescription colorAttachmentDesc = {};
		// // Color attachment
		// colorAttachmentDesc.format = m_ColorFormat;
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

        // vkCreateRenderPass(device, &renderPassInfo, nullptr, &m_RenderPassSwapchain);

        // //! @note Creating framebuffers
        // for (auto& framebuffer : m_Framebuffers)
		// 		vkDestroyFramebuffer(device, framebuffer, nullptr);

        // VkFramebufferCreateInfo frameBufferCreateInfo = {};
        // frameBufferCreateInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        // frameBufferCreateInfo.renderPass = m_RenderPassSwapchain;
        // frameBufferCreateInfo.attachmentCount = 1;
        // frameBufferCreateInfo.width = m_Width;
        // frameBufferCreateInfo.height = m_Height;
        // frameBufferCreateInfo.layers = 1;

        // m_Framebuffers.resize(images_size);
        // for (uint32_t i = 0; i < m_Framebuffers.size(); i++){
        //     frameBufferCreateInfo.pAttachments = &m_SwapchainImages[i].image_view;
        //     vkCreateFramebuffer(device, &frameBufferCreateInfo, nullptr, &m_Framebuffers[i]);
        //     // VKUtils::SetDebugUtilsObjectName(m_Device->GetVulkanDevice(), VK_OBJECT_TYPE_FRAMEBUFFER, std::format("Swapchain framebuffer (Frame in flight: {})", i), m_Framebuffers[i]);
		// }
    }

    void VulkanSwapchain::SearchImageFormatAndColorspace(){
        auto physical_device = VulkanDevice::GetPhysicalDevice().GetVkPhysicalDevice();

        // Get list of supported surface formats
		uint32_t formatCount;
		vkGetPhysicalDeviceSurfaceFormatsKHR(physical_device, m_Surface, &formatCount, NULL);
		// HZ_CORE_ASSERT(formatCount > 0);

		std::vector<VkSurfaceFormatKHR> surfaceFormats(formatCount);
		vkGetPhysicalDeviceSurfaceFormatsKHR(physical_device, m_Surface, &formatCount, surfaceFormats.data());

		// If the surface format list only includes one entry with VK_FORMAT_UNDEFINED,
		// there is no preferered format, so we assume VK_FORMAT_B8G8R8A8_UNORM
		if ((formatCount == 1) && (surfaceFormats[0].format == VK_FORMAT_UNDEFINED)){
			m_ColorFormat = VK_FORMAT_B8G8R8A8_UNORM;
			m_ColorSpace = surfaceFormats[0].colorSpace;
		}
		else{
			// iterate over the list of available surface format and
			// check for the presence of VK_FORMAT_B8G8R8A8_UNORM
			bool found_B8G8R8A8_UNORM = false;
			for (auto&& surfaceFormat : surfaceFormats){
				if (surfaceFormat.format == VK_FORMAT_B8G8R8A8_UNORM){
					m_ColorFormat = surfaceFormat.format;
					m_ColorSpace = surfaceFormat.colorSpace;
					found_B8G8R8A8_UNORM = true;
					break;
				}
			}

			// in case VK_FORMAT_B8G8R8A8_UNORM is not available
			// select the first available color format
			if (!found_B8G8R8A8_UNORM){
				m_ColorFormat = surfaceFormats[0].format;
				m_ColorSpace = surfaceFormats[0].colorSpace;
			}
		}
    }

    uint32_t VulkanSwapchain::AcquireNextImage(){
        //! @note We iterate per-frame while modulizing with the amount of frames per-flight
        m_CurrentFrameIdx = (m_CurrentFrameIdx++) % Renderer::GetSettings().FramesInFlight;
        auto device = VulkanDevice::GetLogicalDevice().GetVkDeviceInstance();
        uint32_t image_idx;
        auto res = vkAcquireNextImageKHR(device, m_CurrentSwapchain, UINT32_MAX, m_ImageAvailableSemaphores[m_CurrentFrameIdx], (VkFence)nullptr, &image_idx);

        if (res != VK_SUCCESS){
			if (res == VK_ERROR_OUT_OF_DATE_KHR || res == VK_SUBOPTIMAL_KHR){
				// OnResize(m_Width, m_Height);
				vkAcquireNextImageKHR(device, m_CurrentSwapchain, UINT64_MAX, m_ImageAvailableSemaphores[m_CurrentFrameIdx], (VkFence)nullptr, &image_idx);
			}
        }
        return image_idx;
    }

    void VulkanSwapchain::Presentation(){
        auto device =VulkanDevice::GetLogicalDevice().GetVkDeviceInstance();

        const uint64_t default_fence_timeout = 100000000000;
        VkPipelineStageFlags waitStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
		
		VkSubmitInfo submitInfo = {};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		submitInfo.pWaitDstStageMask = &waitStageMask;
		submitInfo.pWaitSemaphores = &m_ImageAvailableSemaphores[m_CurrentFrameIdx];
		submitInfo.waitSemaphoreCount = 1;
		submitInfo.pSignalSemaphores = &m_ImagesFinishedRenderingSemaphores[m_CurrentFrameIdx];
		submitInfo.signalSemaphoreCount = 1;
		submitInfo.pCommandBuffers = &m_CommandBuffers[m_CurrentFrameIdx].CommndBufferData;
		submitInfo.commandBufferCount = 1;

		vkResetFences(device, 1, &m_Fences[m_CurrentFrameIdx]);

		// m_Device->LockQueue();

        if(m_DoLockForCompute){
            m_ComputeMutex.lock();
        }
        else{
            m_GraphicsMutex.lock();
        }
		vkQueueSubmit(VulkanDevice::GetLogicalDevice().GetVkGraphicsQueue(), 1, &submitInfo, m_Fences[m_CurrentFrameIdx]);
        // if(m_DoLockForCompute){
        //     m_ComputeMutex.unlock();
        // }
        // else{
        //     m_GraphicsMutex.unlock();
        // }
    }
};