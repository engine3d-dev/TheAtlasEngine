#include <Core/EngineLogger.hpp>
#include <Core/internal/Vulkan2Showcase/helper_functions.hpp>
#include <internal/Vulkan2Showcase/VulkanSwapchain.hpp>
#include <limits>
#include <vulkan/vulkan_core.h>
#include <Core/ApplicationInstance.hpp>
#include <array>

namespace engine3d::vk{
    VulkanSwapchain::VulkanSwapchain(VulkanPhysicalDriver p_PhysicalDriver, VulkanDriver p_Driver, VkSurfaceKHR p_Surface) : m_Driver(p_Driver){
        //! @note This gives us the queue to present/render to display.

        //! @note We extract the current presentation index from our current selected physical index.
        m_PresentationIndex = p_PhysicalDriver.GetPresentationIndex(p_Surface);
        vkGetDeviceQueue(m_Driver, m_PresentationIndex, 0, &m_PresentationQueue);

        uint32_t format_count;
        std::vector<VkSurfaceFormatKHR> formats;

        //! @note Extracting the surface format that is supported on our current GPU (physical device)
        vk_check(vkGetPhysicalDeviceSurfaceFormatsKHR(p_PhysicalDriver, p_Surface, &format_count, nullptr), "vkGetPhysicalDeviceSurfaceFormatsKHR", __FILE__, __LINE__, __FUNCTION__);

        formats.resize(format_count);
        vk_check(vkGetPhysicalDeviceSurfaceFormatsKHR(p_PhysicalDriver, p_Surface, &format_count, formats.data()), "vkGetPhysicalDeviceSurfaceFormatsKHR", __FILE__, __LINE__, __FUNCTION__);

        if(format_count == 1 and formats[0].format == VK_FORMAT_UNDEFINED){
            m_SurfaceFormat.format = VK_FORMAT_B8G8R8A8_UNORM;
            // m_SurfaceFormat.format = VK_FORMAT_R32G32B32A32_SFLOAT;
        }
        else{
            m_SurfaceFormat = formats[0];
        }

        //! @note Getting surface capabilities
        VkSurfaceCapabilitiesKHR surface_capabilities;
        vk_check(vkGetPhysicalDeviceSurfaceCapabilitiesKHR(p_PhysicalDriver, p_Surface, &surface_capabilities), "vkGetPhysicalDeviceSurfaceCapabilitiesKHR", __FILE__, __LINE__, __FUNCTION__);

        //! @note Extracting number of presentation modes.
        uint32_t preset_modes_count;
        std::vector<VkPresentModeKHR> presentation_modes;
        vk_check(vkGetPhysicalDeviceSurfacePresentModesKHR(p_PhysicalDriver, p_Surface, &preset_modes_count, nullptr), "vkGetPhysicalDeviceSurfacePresentModesKHR", __FILE__, __LINE__, __FUNCTION__);
        presentation_modes.resize(preset_modes_count);
        vk_check(vkGetPhysicalDeviceSurfacePresentModesKHR(p_PhysicalDriver, p_Surface, &preset_modes_count, presentation_modes.data()), "vkGetPhysicalDeviceSurfacePresentModesKHR", __FILE__, __LINE__, __FUNCTION__);

        m_PresentMode = SelectCompatiblePresentMode(VK_PRESENT_MODE_IMMEDIATE_KHR, presentation_modes);
        m_SwapchainExtent = SelectValidExtent(surface_capabilities);

        // Extract min num of images supported on our current surface.
        uint32_t surface_img_count = std::max<uint32_t>(3, surface_capabilities.minImageCount);

        //! @note Populating swapchain creation into info struct
        VkSwapchainCreateInfoKHR create_info = {
            .sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
            .pNext = nullptr,
            .flags = 0,
            .surface = p_Surface,
            .imageFormat = m_SurfaceFormat.format,
            .preTransform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR,
        };

        if((surface_capabilities.supportedTransforms & VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR) == 0){
            bool expression = (surface_capabilities.supportedTransforms & VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR);
            if(!expression){
                ConsoleLogError("Surface does not support VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR transformation");
                assert(false);
            }
        }

        create_info.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
        create_info.presentMode = m_PresentMode;
        create_info.minImageCount = surface_img_count;
        create_info.oldSwapchain = nullptr;
        create_info.clipped = true;
        create_info.imageExtent = {
            .width = m_SwapchainExtent.width,
            .height = m_SwapchainExtent.height
        };

        create_info.imageArrayLayers = 1;
        create_info.imageColorSpace = m_SurfaceFormat.colorSpace;
        create_info.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
        // ConsoleLogError("Just after create_info.imageUsage called!");
        // ConsoleLogError("App Width == {}", ApplicationInstance::GetWindow().GetWidth());
        // ConsoleLogError("App height == {}", ApplicationInstance::GetWindow().GetHeight());
        //! @note Fixing width/height of surface in case they aren't defined.
        // if(ApplicationInstance::GetWindow().GetWidth() == 0 || ApplicationInstance::GetWindow().GetHeight() == 0){
        //     ConsoleLogError("Application Width/Height === 0");
        //     create_info.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
        //     create_info.queueFamilyIndexCount = 0;
        //     create_info.pQueueFamilyIndices = nullptr;
        // }
        // else{
        //     ConsoleLogError("Application Width/height not 0!");
        //     uint32_t queue_fam_indices[] = {p_PhysicalDriver.GetQueueIndices().Graphics, m_PresentationIndex};
        //     create_info.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
        //     create_info.queueFamilyIndexCount = 2;
        //     create_info.pQueueFamilyIndices = queue_fam_indices;
        // }

        //! @note Instantiating swapchain
        vk_check(vkCreateSwapchainKHR(p_Driver, &create_info, nullptr, &m_Swapchain), "vkCreateSwapchainKHR", __FILE__, __LINE__, __FUNCTION__);

        ConsoleLogWarn("Vulkan2Showcase: Vulkan Swapchain Initialized Complete!!!");

        ConsoleLogInfo("Vulkan2Showcase: Begin VulkanSwapchain Images Initialization");

        uint32_t swapchain_images_count;
        std::vector<VkImage> images;

        //! @note Getting amount of images for this swapchain.
        vk_check(vkGetSwapchainImagesKHR(p_Driver, m_Swapchain, &swapchain_images_count, nullptr), "vkGetSwapchainImagesKHR", __FILE__, __LINE__, __FUNCTION__);

        images.resize(swapchain_images_count);

        //! @note Keep tracking of our actual swapchain images our swapchain will actually utilize.
        m_ImagesForSwapchain.resize(swapchain_images_count);
        // extracting all of our images 
        vk_check(vkGetSwapchainImagesKHR(p_Driver, m_Swapchain, &swapchain_images_count, images.data()), "vkGetSwapchainImagesKHR", __FILE__, __LINE__, __FUNCTION__);
        
        for(uint32_t i = 0; i < swapchain_images_count; i++){
            m_ImagesForSwapchain[i].Image = images[i];

            VkImageViewCreateInfo image_view_create_info = {
                .sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
                .pNext = nullptr,
                .flags = 0,
                .image = m_ImagesForSwapchain[i].Image,
                .viewType = VK_IMAGE_VIEW_TYPE_2D,
                .format = m_SurfaceFormat.format,
                .components = {
                    .r = VK_COMPONENT_SWIZZLE_R,
                    .g = VK_COMPONENT_SWIZZLE_G,
                    .b = VK_COMPONENT_SWIZZLE_B
                },
                .subresourceRange = {
                    .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
                    .baseMipLevel = 0,
                    .levelCount = 1,
                    .baseArrayLayer = 0,
                    .layerCount = 1,
                }
            };

            vk_check(vkCreateImageView(p_Driver, &image_view_create_info, nullptr, &m_ImagesForSwapchain[i].ImageView), "vkCreateImageView", __FILE__, __LINE__, __FUNCTION__);
        }

        ConsoleLogWarn("Vulkan2Showcase: Image Initiated Completed!!!");

        //! @note Setting up Command Buffers.

        ConsoleLogInfo("Vulkan2Showcase: Begin Command Buffers Initiated!!");
        VkCommandPoolCreateInfo cmd_pool_create_info = {
            .sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
            .pNext = nullptr,
            .flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT,
            .queueFamilyIndex = p_PhysicalDriver.GetQueueIndices().Graphics,
        };

        vk_check(vkCreateCommandPool(p_Driver, &cmd_pool_create_info, nullptr, &m_CommandPoolForSwapchain), "vkCreateCommandPool", __FILE__, __LINE__, __FUNCTION__);

        m_SwapchainCommandBuffers.resize(m_ImagesForSwapchain.size());

        VkCommandBufferAllocateInfo cmd_buffer_alloc_info = {
            .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
            .pNext = nullptr,
            .commandPool = m_CommandPoolForSwapchain,
            .level = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
            .commandBufferCount = static_cast<uint32_t>(m_SwapchainCommandBuffers.size()),
        };

        vk_check(vkAllocateCommandBuffers(p_Driver, &cmd_buffer_alloc_info, m_SwapchainCommandBuffers.data()), "vkAllocateCommandBuffers", __FILE__, __LINE__, __FUNCTION__);

        ConsoleLogWarn("Vulkan2Showcase: Vulkan Swapchain CommandBuffers Initiated Completed!!");


        //! @note Setting up swapchain render pass here.
        //! @note This render pass is ONLY specifically for rendering the images and indicating when they are finished.
        ConsoleLogInfo("Vulkan2Showcase: Creating VulkanSwapchain RenderPass!");
        VkAttachmentDescription depth_attachments_description = {
            .format = SelectDepthFormat(p_PhysicalDriver),
            .samples = VK_SAMPLE_COUNT_1_BIT,
            .loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR,
            .storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE,
            .stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE,
            .stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE,
            .initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
            .finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL
        };

        VkAttachmentReference depth_attachment_ref = {
            .attachment = 1,
            .layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL
        };

        VkAttachmentDescription color_attachment_description = {
            .format = GetSwapchainFormat(),
            .samples = VK_SAMPLE_COUNT_1_BIT,
            .loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR,
            .storeOp = VK_ATTACHMENT_STORE_OP_STORE,
            .stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE,
            .stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE,
            .initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
            .finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR
        };

        VkAttachmentReference color_attachment_ref = {
            .attachment = 0,
            .layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL
        };

        VkSubpassDescription subpass_description = {
            .pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS,
            .colorAttachmentCount = 1,
            .pColorAttachments = &color_attachment_ref,
            .pDepthStencilAttachment = &depth_attachment_ref
        };

        VkSubpassDependency subpass_dependency = {
            .srcSubpass = VK_SUBPASS_EXTERNAL,
            .dstSubpass = 0,
            .srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT,
            .dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT,
            .srcAccessMask = 0,
            .dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT
        };

        std::array<VkAttachmentDescription, 2> attachments = { color_attachment_description, depth_attachments_description };

        VkRenderPassCreateInfo renderpass_create_info = {
            .sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO,
            .attachmentCount = static_cast<uint32_t>(attachments.size()),
            .pAttachments = attachments.data(),
            .subpassCount = 1,
            .pSubpasses = &subpass_description,
            .dependencyCount = 1,
            .pDependencies = &subpass_dependency
        };

        vk_check(vkCreateRenderPass(p_Driver, &renderpass_create_info, nullptr, &m_RenderpassForSwapchain), "vkCreateRenderPass", __FILE__, __LINE__, __FUNCTION__);

        ConsoleLogWarn("Vulkan2Showcase: Vulkan Swapchain Renderpass Initialized Completed!!!");


        //! @note Initializing Depth Formats.
        ConsoleLogInfo("Vulkan2Showcase: Begin Vulkan Swapchain Depth Images for swapchains Initializing...");
        VkFormat selected_depth_format = SelectDepthFormat(p_PhysicalDriver);
        VkExtent2D swapchain_extent = m_SwapchainExtent;

        //! @note Making sure our depth images are the same as our swapchains amount of images this swapchain can accept.
        m_DepthImagesForSwapchain.resize(GetImagesSize());

        // Creating our images for depth view to lay-over

        //! @note We want to set the depth images to this memory property flag.
        //! @note To specify memory allocation with this type for most efficient for specifically device accesses.

        VkPhysicalDeviceMemoryProperties memory_properties;
        vkGetPhysicalDeviceMemoryProperties(p_PhysicalDriver, &memory_properties);

        VkMemoryPropertyFlags properties_flag = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
        for(size_t i = 0; i < m_DepthImagesForSwapchain.size(); i++){
            VkImageCreateInfo image_create_info = {
                .sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO,
                .pNext = nullptr,
                .flags = 0,
                .format = selected_depth_format,
                .extent = { .width = swapchain_extent.width, .height = swapchain_extent.height, .depth = 1 },
                .mipLevels = 1,
                .arrayLayers = 1,
                .samples = VK_SAMPLE_COUNT_1_BIT,
                .tiling = VK_IMAGE_TILING_OPTIMAL,
                .usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT,
                .sharingMode = VK_SHARING_MODE_EXCLUSIVE,
                .initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
            };

            vk_check(vkCreateImage(p_Driver, &image_create_info, nullptr, &m_DepthImagesForSwapchain[i].Image), "vkCreateImage", __FILE__, __LINE__, __FUNCTION__);

            VkMemoryRequirements memory_requirements;
            vkGetImageMemoryRequirements(p_Driver, m_DepthImagesForSwapchain[i].Image, &memory_requirements);
            
            VkMemoryAllocateInfo mem_alloc_info = {
                .sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
                .allocationSize = memory_requirements.size,
                .memoryTypeIndex = SelectMemoryType(memory_properties, memory_requirements.memoryTypeBits, properties_flag)
            };

            vk_check(vkAllocateMemory(p_Driver, &mem_alloc_info, nullptr, &m_DepthImagesForSwapchain[i].DeviceMemory), "vkAllocateMemory", __FILE__, __LINE__, __FUNCTION__);

            vk_check(vkBindImageMemory(p_Driver, m_DepthImagesForSwapchain[i].Image, m_DepthImagesForSwapchain[i].DeviceMemory, 0), "vkBindImageMemory", __FILE__, __LINE__, __FUNCTION__);
        
            VkImageViewCreateInfo image_view_create_info = {
                .sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
                .image = m_DepthImagesForSwapchain[i].Image,
                .viewType = VK_IMAGE_VIEW_TYPE_2D,
                .format = selected_depth_format,
                .subresourceRange = {
                    .aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT,
                    .baseMipLevel = 0,
                    .levelCount = 1,
                    .baseArrayLayer = 0,
                    .layerCount = 0
                }
            };

            vk_check(vkCreateImageView(p_Driver, &image_view_create_info, nullptr, &m_DepthImagesForSwapchain[i].ImageView), "vkCreateImageView", __FILE__, __LINE__, __FUNCTION__);
        }

        ConsoleLogWarn("Vulkan2Showcase: Depth Images Initialized Completed!!");


        //! @note Setting up framebuffers for swapchain.
        ConsoleLogInfo("Vulkan2Showcase: Begin Framebuffers Initialization!");
        m_FramebuffersForSwapchain.resize(GetImagesSize());

        for(size_t i = 0; i < GetImagesSize(); i++){
            std::array<VkImageView, 2> attachments = { m_ImagesForSwapchain[i].ImageView, m_DepthImagesForSwapchain[i].ImageView};

            VkExtent2D swapchain_extent = m_SwapchainExtent;
            VkFramebufferCreateInfo fb_create_info = {
                .sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO,
                .pNext = nullptr,
                .flags = 0,
                .renderPass = m_RenderpassForSwapchain,
                .attachmentCount = static_cast<uint32_t>(attachments.size()),
                .pAttachments = attachments.data(),
                .width = swapchain_extent.width,
                .height = swapchain_extent.height,
                .layers = 1
            };

            vk_check(vkCreateFramebuffer(p_Driver, &fb_create_info, nullptr, &m_FramebuffersForSwapchain[i]), "vkCreateFramebuffer", __FILE__, __LINE__, __FUNCTION__);
        }

        ConsoleLogWarn("Vulkan2Showcase: Vulkan Framebuffers Initialization Completed!!!!");


        //! @note Setting up Syncrhonization Objects for semaphores/fences.
        ConsoleLogInfo("Vulkan2Showcase: Begin Vulkan Sync Objects Initialization");

        //! @note Setting up our arrays
        m_SemaphoresForAvailableImages.resize(MaxFramesInFlight);
        m_SemaphoresForRenderCompleted.resize(MaxFramesInFlight);
        m_InFlightFences.resize(MaxFramesInFlight);
        m_ImagesInFlight.resize(GetImagesSize(), VK_NULL_HANDLE);

        VkSemaphoreCreateInfo semaphore_create_info = {
            .sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO
        };

        VkFenceCreateInfo fence_create_info = {
            .sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO,
            .flags = VK_FENCE_CREATE_SIGNALED_BIT
        };

        for(size_t i = 0; i < MaxFramesInFlight; i++){
            vk_check(vkCreateSemaphore(p_Driver, &semaphore_create_info, nullptr, &m_SemaphoresForAvailableImages[i]), "vkCreateSemaphore (1)", __FILE__, __LINE__, __FUNCTION__);

            vk_check(vkCreateSemaphore(p_Driver, &semaphore_create_info, nullptr, &m_SemaphoresForRenderCompleted[i]), "vkCreateSemaphore (2)", __FILE__, __LINE__, __FUNCTION__);

            vk_check(vkCreateFence(p_Driver, &fence_create_info, nullptr, &m_InFlightFences[i]), "vkCreateFence", __FILE__, __LINE__, __FUNCTION__);
        }

        ConsoleLogWarn("Vulkan2Showcase: Vulkan Swapchain Sync Objects Initialized Completed!!!!");

        ConsoleLogWarn("Vulkan2Showcase: Vulkan Swapchain Initialized Complete");
    }

    VkSwapchainKHR VulkanSwapchain::VkSwapchainHandler() {
        return m_Swapchain;
    }

    VkFormat VulkanSwapchain::ReadSwapchainFormat(){
        return m_SurfaceFormat.format;
    }
    
    uint32_t VulkanSwapchain::ImagesSize() const {
        return m_ImagesForSwapchain.size();
    }
    
    VkFramebuffer VulkanSwapchain::ReadFramebuffer(uint32_t index) {
        return m_FramebuffersForSwapchain[index];
    }
    
    VkImageView VulkanSwapchain::ReadImageView(uint32_t index) {
        return m_ImagesForSwapchain[index].ImageView;
    }
    
    VkExtent2D VulkanSwapchain::ReadSwapchainExtent() {
        return m_SwapchainExtent;
    }

    VkRenderPass VulkanSwapchain::ReadSwapchainRenderPass(){
        return m_RenderpassForSwapchain;
    }
    
    void VulkanSwapchain::SubmitAndWriteCommandBuffer(VkCommandBuffer* p_CommandBuffers) {
        if(m_ImagesInFlight[m_CurrentImageIndex] != VK_NULL_HANDLE){
            vkWaitForFences(m_Driver, 1, &m_ImagesInFlight[m_CurrentImageIndex], true, std::numeric_limits<uint64_t>::max());
        }

        m_ImagesInFlight[m_CurrentImageIndex] = m_InFlightFences[m_CurrentFrameIndex];

        VkSubmitInfo submit_info = {
            .sType = VK_STRUCTURE_TYPE_SUBMIT_INFO,
            .pNext = nullptr,
        };

        VkSemaphore wait_semaphore[] = {m_SemaphoresForAvailableImages[m_CurrentFrameIndex]};

        VkPipelineStageFlags wait_stages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};

        submit_info.waitSemaphoreCount = 1;
        submit_info.pWaitSemaphores = wait_semaphore;
        submit_info.pWaitDstStageMask = wait_stages;

        submit_info.commandBufferCount = 1;
        submit_info.pCommandBuffers = p_CommandBuffers;

        VkSemaphore signal_sems[] = { m_SemaphoresForRenderCompleted[m_CurrentFrameIndex]};

        submit_info.signalSemaphoreCount = 1;
        submit_info.pSignalSemaphores = signal_sems;

        vkResetFences(m_Driver, 1, &m_InFlightFences[m_CurrentFrameIndex]);

        vk_check(vkQueueSubmit(m_Driver.GetGraphicsQueue(), 1, &submit_info, m_InFlightFences[m_CurrentFrameIndex]), "vkQueueSubmit", __FILE__, __LINE__, __FUNCTION__);

        //! @note Now we present our data to the display.
        VkPresentInfoKHR present_info = {
            .sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR,
            .waitSemaphoreCount = 1,
            .pWaitSemaphores = signal_sems,
        };

        VkSwapchainKHR swapchains_array[] = { m_Swapchain };
        present_info.swapchainCount = 1;
        present_info.pSwapchains = swapchains_array;

        present_info.pImageIndices = &m_CurrentImageIndex;
        if(m_PresentationQueue == VK_NULL_HANDLE){
            ConsoleLogError("PresentationQueue is nullptr!!!");
        }

        vk_check(vkQueuePresentKHR(m_PresentationQueue, &present_info), "vkQueuePresentKHR", __FILE__, __LINE__, __FUNCTION__);

        m_CurrentFrameIndex = (m_CurrentFrameIndex + 1) % VulkanSwapchain::MaxFramesInFlight;
    }
    
    uint32_t VulkanSwapchain::ReadAcquiredNextFrame() {
        uint32_t image_index;
        vk_check(vkWaitForFences(m_Driver, 1, &m_InFlightFences[m_CurrentFrameIndex], true, std::numeric_limits<uint32_t>::max()), "vkWaitForFences", __FILE__, __LINE__, __FUNCTION__);

        vk_check(vkAcquireNextImageKHR(m_Driver, m_Swapchain, std::numeric_limits<uint64_t>::max(), m_SemaphoresForAvailableImages[m_CurrentFrameIndex], VK_NULL_HANDLE, &image_index), "vkAcquireNextImageKHR", __FILE__, __LINE__, __FUNCTION__);
        m_CurrentImageIndex = image_index;
        return image_index;
    }

    uint32_t VulkanSwapchain::CurrentFramePerTick() {
        return m_CurrentFrameIndex;
    }











    //! @note Extracts the next image.
    /*
    uint32_t VulkanSwapchain::AcquireNextImage(){
        uint32_t image_index;
        vk_check(vkWaitForFences(m_Driver, 1, &m_InFlightFences[m_CurrentFrameIndex], true, std::numeric_limits<uint32_t>::max()), "vkWaitForFences", __FILE__, __LINE__, __FUNCTION__);

        vk_check(vkAcquireNextImageKHR(m_Driver, m_Swapchain, std::numeric_limits<uint64_t>::max(), m_SemaphoresForAvailableImages[m_CurrentFrameIndex], VK_NULL_HANDLE, &image_index), "vkAcquireNextImageKHR", __FILE__, __LINE__, __FUNCTION__);
        m_CurrentImageIndex = image_index;
        return image_index;
    }
    */

    /*
    void VulkanSwapchain::SubmitCommandBuffers(VkCommandBuffer* p_CommandBuffers){
        if(m_ImagesInFlight[m_CurrentImageIndex] != VK_NULL_HANDLE){
            vkWaitForFences(m_Driver, 1, &m_ImagesInFlight[m_CurrentImageIndex], true, std::numeric_limits<uint64_t>::max());
        }

        m_ImagesInFlight[m_CurrentImageIndex] = m_InFlightFences[m_CurrentFrameIndex];

        VkSubmitInfo submit_info = {
            .sType = VK_STRUCTURE_TYPE_SUBMIT_INFO,
            .pNext = nullptr,
        };

        VkSemaphore wait_semaphore[] = {m_SemaphoresForAvailableImages[m_CurrentFrameIndex]};

        VkPipelineStageFlags wait_stages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};

        submit_info.waitSemaphoreCount = 1;
        submit_info.pWaitSemaphores = wait_semaphore;
        submit_info.pWaitDstStageMask = wait_stages;

        submit_info.commandBufferCount = 1;
        submit_info.pCommandBuffers = p_CommandBuffers;

        VkSemaphore signal_sems[] = { m_SemaphoresForRenderCompleted[m_CurrentFrameIndex]};

        submit_info.signalSemaphoreCount = 1;
        submit_info.pSignalSemaphores = signal_sems;

        vkResetFences(m_Driver, 1, &m_InFlightFences[m_CurrentFrameIndex]);

        vk_check(vkQueueSubmit(m_Driver.GetGraphicsQueue(), 1, &submit_info, m_InFlightFences[m_CurrentFrameIndex]), "vkQueueSubmit", __FILE__, __LINE__, __FUNCTION__);

        //! @note Now we present our data to the display.
        VkPresentInfoKHR present_info = {
            .sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR,
            .waitSemaphoreCount = 1,
            .pWaitSemaphores = signal_sems,
        };

        VkSwapchainKHR swapchains_array[] = { m_Swapchain };
        present_info.swapchainCount = 1;
        present_info.pSwapchains = swapchains_array;

        present_info.pImageIndices = &m_CurrentImageIndex;
        if(m_PresentationQueue == VK_NULL_HANDLE){
            ConsoleLogError("PresentationQueue is nullptr!!!");
        }

        vk_check(vkQueuePresentKHR(m_PresentationQueue, &present_info), "vkQueuePresentKHR", __FILE__, __LINE__, __FUNCTION__);

        m_CurrentFrameIndex = (m_CurrentFrameIndex + 1) % VulkanSwapchain::MaxFramesInFlight;
    }
    */





    uint32_t VulkanSwapchain::SelectMemoryType(VkPhysicalDeviceMemoryProperties p_MemoryProperties, uint32_t p_TypeFilter, VkMemoryPropertyFlags property_flag){
        for(uint32_t i = 0; i < p_MemoryProperties.memoryTypeCount; i++){
            if((p_TypeFilter & (1 << i)) && (p_MemoryProperties.memoryTypes[i].propertyFlags & property_flag) == property_flag){
                return i;
            }
        }
        ConsoleLogError("VulkanSwapchain::SelectMemoryType returns zero meaning failed to find suitable memory type");
        return 0;
    }

    VkPresentModeKHR VulkanSwapchain::SelectCompatiblePresentMode(const VkPresentModeKHR& p_RequestMode, const std::vector<VkPresentModeKHR>& p_Modes){
        for(const auto& mode : p_Modes){
            if(mode == p_RequestMode){
                return mode;
            }
        }
        return VK_PRESENT_MODE_FIFO_KHR;
    }

    VkExtent2D VulkanSwapchain::SelectValidExtent(const VkSurfaceCapabilitiesKHR& p_SurfaceCapabilities){
        //! @note Width/Height of our current swapchain
        if(p_SurfaceCapabilities.currentExtent.width == std::numeric_limits<uint32_t>::max() || p_SurfaceCapabilities.currentExtent.height == std::numeric_limits<uint32_t>::max()){
            //! @note Passing width and height from passed in VkSurface.
            m_SwapchainExtent = p_SurfaceCapabilities.currentExtent;
            VkExtent2D current_extent = m_SwapchainExtent;

            //! @note Swapchain Extent checking so that width/height are not less than minimum width/height or greater then the max width/height
            if(m_SwapchainExtent.width < p_SurfaceCapabilities.minImageExtent.width){
                current_extent.width = p_SurfaceCapabilities.minImageExtent.width;
            }
            else if(m_SwapchainExtent.width > p_SurfaceCapabilities.maxImageExtent.width){
                current_extent.width = p_SurfaceCapabilities.maxImageExtent.width;
            }


            if(m_SwapchainExtent.height < p_SurfaceCapabilities.minImageExtent.height){
                current_extent.height = p_SurfaceCapabilities.minImageExtent.height;
            }
            else if(m_SwapchainExtent.height > p_SurfaceCapabilities.maxImageExtent.height){
                current_extent.height = p_SurfaceCapabilities.maxImageExtent.height;
            }

            if(current_extent.width == 0 and current_extent.height == 0){
                current_extent.width = ApplicationInstance::GetWindow().GetWidth();
                current_extent.height = ApplicationInstance::GetWindow().GetHeight();
            }

            return current_extent;
        }

        if(p_SurfaceCapabilities.currentExtent.width == 0 and p_SurfaceCapabilities.currentExtent.height){
            VkExtent2D current_extent;
            current_extent.width = ApplicationInstance::GetWindow().GetWidth();
            current_extent.height = ApplicationInstance::GetWindow().GetHeight();
        }
        return p_SurfaceCapabilities.currentExtent;
    }

    VkFormat VulkanSwapchain::SelectDepthFormat(VkPhysicalDevice p_PhysicalDevice){
        return SelectSupportedFormat(p_PhysicalDevice,
                                {VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT},
                                VK_IMAGE_TILING_OPTIMAL,
                    VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT);
        

    }

    VkFormat VulkanSwapchain::SelectSupportedFormat(VkPhysicalDevice p_PhysicalDevice, const std::vector<VkFormat>& p_Formats, VkImageTiling p_Tiling, VkFormatFeatureFlags p_FeatureFlag){
        VkFormat valid_format = VK_FORMAT_UNDEFINED;
        
        for(const auto& format : p_Formats){
            VkFormatProperties properties;
            vkGetPhysicalDeviceFormatProperties(p_PhysicalDevice, format, &properties);

            if(p_Tiling == VK_IMAGE_TILING_LINEAR && (properties.linearTilingFeatures & p_FeatureFlag) == p_FeatureFlag){
                valid_format = format;
            }
            else if(p_Tiling == VK_IMAGE_TILING_OPTIMAL && (properties.optimalTilingFeatures & p_FeatureFlag) == p_FeatureFlag){
                valid_format = format;
            }
        }

        vk_check_format(valid_format, __FILE__, __LINE__, __FUNCTION__);

        return valid_format;
    }
};