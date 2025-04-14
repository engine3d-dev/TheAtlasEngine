#include <drivers/vulkan/vulkan_swapchain.hpp>
#include <core/engine_logger.hpp>
#include <core/application.hpp>
#include <drivers/vulkan/helper_functions.hpp>
#include <vulkan/vulkan_core.h>

namespace atlas::vk {
    static bool g_is_swapchain_resized = false;

    vk_swapchain::vk_swapchain(const vk_physical_driver& p_physical_driver,
                               const vk_driver& p_driver,
                               const VkSurfaceKHR& p_surface)
      : m_current_surface(p_surface)
      , m_physical(p_physical_driver)
      , m_driver(p_driver) {
        //! @note This gives us the queue to present/render to display.

        //! @note We extract the current presentation index from our current
        //! selected physical index.
        m_presentation_index =
          p_physical_driver.get_presentation_index(p_surface);
        vkGetDeviceQueue(
          m_driver, m_presentation_index, 0, &m_presentation_queue);
        on_create(application::get_window().get_width(),
                  application::get_window().get_height());
    }

    void vk_swapchain::on_create(uint32_t p_width, const uint32_t p_height) {
        m_width = p_width;
        m_height = p_height;

        uint32_t format_count;
        std::vector<VkSurfaceFormatKHR> formats;

        //! @note Extracting the surface format that is supported on our current
        //! GPU (physical device)
        vk_check(vkGetPhysicalDeviceSurfaceFormatsKHR(
                   m_physical, m_current_surface, &format_count, nullptr),
                 "vkGetPhysicalDeviceSurfaceFormatsKHR",
                 __FILE__,
                 __LINE__,
                 __FUNCTION__);

        formats.resize(format_count);
        vk_check(
          vkGetPhysicalDeviceSurfaceFormatsKHR(
            m_physical, m_current_surface, &format_count, formats.data()),
          "vkGetPhysicalDeviceSurfaceFormatsKHR",
          __FILE__,
          __LINE__,
          __FUNCTION__);

        if (format_count == 1 and formats[0].format == VK_FORMAT_UNDEFINED) {
            m_surface_format.format = VK_FORMAT_B8G8R8A8_UNORM;
            // m_surface_format.format = VK_FORMAT_R32G32B32A32_SFLOAT;
        }
        else {
            m_surface_format = formats[0];
        }

        //! @note Getting surface capabilities
        VkSurfaceCapabilitiesKHR surface_capabilities;
        vk_check(vkGetPhysicalDeviceSurfaceCapabilitiesKHR(
                   m_physical, m_current_surface, &surface_capabilities),
                 "vkGetPhysicalDeviceSurfaceCapabilitiesKHR",
                 __FILE__,
                 __LINE__,
                 __FUNCTION__);

        //! @note Extracting number of presentation modes.
        uint32_t preset_modes_count;
        std::vector<VkPresentModeKHR> presentation_modes;
        vk_check(vkGetPhysicalDeviceSurfacePresentModesKHR(
                   m_physical, m_current_surface, &preset_modes_count, nullptr),
                 "vkGetPhysicalDeviceSurfacePresentModesKHR",
                 __FILE__,
                 __LINE__,
                 __FUNCTION__);
        presentation_modes.resize(preset_modes_count);
        vk_check(
          vkGetPhysicalDeviceSurfacePresentModesKHR(m_physical,
                                                    m_current_surface,
                                                    &preset_modes_count,
                                                    presentation_modes.data()),
          "vkGetPhysicalDeviceSurfacePresentModesKHR",
          __FILE__,
          __LINE__,
          __FUNCTION__);

        m_presentation_mode = select_compatible_present_mode(
          VK_PRESENT_MODE_IMMEDIATE_KHR, presentation_modes);
        m_swapchain_extent = select_valid_extent(surface_capabilities);

        // Extract min num of images supported on our current surface.
        uint32_t surface_img_count =
          std::max<uint32_t>(3, surface_capabilities.minImageCount);

        //! @note Populating swapchain creation into info struct
        VkSwapchainCreateInfoKHR create_info = {
            .sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
            .pNext = nullptr,
            .flags = 0,
            .surface = m_current_surface,
            .minImageCount = 2,
            .imageFormat = m_surface_format.format,
            .preTransform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR
        };

        if ((surface_capabilities.supportedTransforms &
             VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR) == 0) {
            bool expression = (surface_capabilities.supportedTransforms &
                               VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR);
            if (!expression) {
                console_log_error(
                  "Surface does not support "
                  "VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR transformation");
                assert(false);
            }
        }

        create_info.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
        create_info.presentMode = m_presentation_mode;
        create_info.minImageCount = surface_img_count;
        create_info.oldSwapchain = nullptr;
        create_info.clipped = true;
        create_info.imageExtent = { .width = m_swapchain_extent.width,
                                    .height = m_swapchain_extent.height };

        create_info.imageArrayLayers = 1;
        create_info.imageColorSpace = m_surface_format.colorSpace;
        create_info.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

        // VkSwapchainKHR old_swapchain = VK_NULL_HANDLE;
        if (m_swapchain != VK_NULL_HANDLE and m_is_resized_requested) {
            // old_swapchain = m_swapchain;
            vkDestroySwapchainKHR(m_driver, m_swapchain, nullptr);
        }

        //! @note Instantiating swapchain
        vk_check(
          vkCreateSwapchainKHR(m_driver, &create_info, nullptr, &m_swapchain),
          "vkCreateSwapchainKHR",
          __FILE__,
          __LINE__,
          __FUNCTION__);

        // console_log_warn("Vulkan2Showcase: Vulkan swapchain Initialized
        // Complete!!!");
        console_log_warn_tagged("vulkan",
                                "Vulkan swapchain Initialized Complete!!!");

        // console_log_info("Vulkan2Showcase: Begin vk_swapchain Images
        // Initialization");
        console_log_warn_tagged("vulkan",
                                "Begin vk_swapchain Images Initialization");

        uint32_t swapchain_images_count;
        std::vector<VkImage> images;

        //! @note Getting amount of images for this swapchain.
        vk_check(vkGetSwapchainImagesKHR(
                   m_driver, m_swapchain, &swapchain_images_count, nullptr),
                 "vkGetSwapchainImagesKHR",
                 __FILE__,
                 __LINE__,
                 __FUNCTION__);

        images.resize(swapchain_images_count);

        //! @note Keep tracking of our actual swapchain images our swapchain
        //! will actually utilize.
        m_swapchain_images.resize(swapchain_images_count);
        // extracting all of our images
        vk_check(
          vkGetSwapchainImagesKHR(
            m_driver, m_swapchain, &swapchain_images_count, images.data()),
          "vkGetSwapchainImagesKHR",
          __FILE__,
          __LINE__,
          __FUNCTION__);

        for (uint32_t i = 0; i < swapchain_images_count; i++) {
            m_swapchain_images[i].Image = images[i];

            VkImageViewCreateInfo image_view_create_info = {
                .sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
                .pNext = nullptr,
                .flags = 0,
                .image = m_swapchain_images[i].Image,
                .viewType = VK_IMAGE_VIEW_TYPE_2D,
                .format = m_surface_format.format,
                .components = {
                  .r = VK_COMPONENT_SWIZZLE_R,
                  .g = VK_COMPONENT_SWIZZLE_G,
                  .b = VK_COMPONENT_SWIZZLE_B,
                },
                .subresourceRange = {
                  .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
                  .baseMipLevel = 0,
                  .levelCount = 1,
                  .baseArrayLayer = 0,
                  .layerCount = 1,
                },
            };

            vk_check(vkCreateImageView(m_driver,
                                       &image_view_create_info,
                                       nullptr,
                                       &m_swapchain_images[i].ImageView),
                     "vkCreateImageView",
                     __FILE__,
                     __LINE__,
                     __FUNCTION__);
        }

        // console_log_warn("Vulkan2Showcase: Image Initiated Completed!!!");
        console_log_warn_tagged("vulkan", "Image Initiated Completed!!!");

        //! @note Setting up Command Buffers.

        console_log_warn_tagged("vulkan", "Begin Command Buffers Initiated!!");
        VkCommandPoolCreateInfo cmd_pool_create_info = {
            .sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
            .pNext = nullptr,
            .flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT,
            .queueFamilyIndex = m_physical.get_queue_indices().Graphics,
        };

        vk_check(vkCreateCommandPool(m_driver,
                                     &cmd_pool_create_info,
                                     nullptr,
                                     &m_swapchain_command_pool),
                 "vkCreateCommandPool",
                 __FILE__,
                 __LINE__,
                 __FUNCTION__);

        m_swapchain_command_buffers.resize(m_swapchain_images.size());

        VkCommandBufferAllocateInfo cmd_buffer_alloc_info = {
            .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
            .pNext = nullptr,
            .commandPool = m_swapchain_command_pool,
            .level = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
            .commandBufferCount =
              static_cast<uint32_t>(m_swapchain_command_buffers.size()),
        };

        vk_check(vkAllocateCommandBuffers(m_driver,
                                          &cmd_buffer_alloc_info,
                                          m_swapchain_command_buffers.data()),
                 "vkAllocateCommandBuffers",
                 __FILE__,
                 __LINE__,
                 __FUNCTION__);

        // console_log_warn("Vulkan2Showcase: Vulkan swapchain CommandBuffers
        // Initiated Completed!!");
        console_log_warn_tagged(
          "vulkan", "Vulkan swapchain CommandBuffers Initiated Completed!!");

        //! @note Setting up swapchain render pass here.
        //! @note This render pass is ONLY specifically for rendering the images
        //! and indicating when they are finished.
        // console_log_info("Vulkan2Showcase: Creating vk_swapchain
        // renderpass!");
        console_log_warn_tagged("vulkan", "Creating vk_swapchain renderpass!");
        VkAttachmentDescription depth_attachments_description = {
            .format = select_depth_format(m_physical),
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
            .format = get_swapchain_format(),
            .samples = VK_SAMPLE_COUNT_1_BIT,
            .loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR,
            .storeOp = VK_ATTACHMENT_STORE_OP_STORE,
            .stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE,
            .stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE,
            .initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
            .finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR
        };

        VkAttachmentReference color_attachment_ref = {
            .attachment = 0, .layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL
        };

        VkSubpassDescription subpass_description = {
            .pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS,
            .colorAttachmentCount = 1,
            .pColorAttachments = &color_attachment_ref,
            .pDepthStencilAttachment = &depth_attachment_ref
        };

        //! @note Subpass depencies are actually optional
        //! @note Subpass dependencies are actuall optional!! So we can leave
        //! this out
        // VkSubpassDependency subpass_dependency = {
        //     .srcSubpass = VK_SUBPASS_EXTERNAL,
        //     .dstSubpass = 0,
        //     .srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT |
        //     VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT, .dstStageMask =
        //     VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT |
        //     VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT, .srcAccessMask = 0,
        //     .dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT |
        //     VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT
        // };

        std::array<VkAttachmentDescription, 2> attachments = {
            color_attachment_description, depth_attachments_description
        };

        VkRenderPassCreateInfo renderpass_create_info = {
            .sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO,
            .attachmentCount = static_cast<uint32_t>(attachments.size()),
            .pAttachments = attachments.data(),
            .subpassCount = 1,
            .pSubpasses = &subpass_description,
            .dependencyCount = 0,
            .pDependencies = nullptr
            // .dependencyCount = 1,
            // .pDependencies = &subpass_dependency
        };

        vk_check(vkCreateRenderPass(m_driver,
                                    &renderpass_create_info,
                                    nullptr,
                                    &m_swapchain_renderpass),
                 "vkCreateRenderPass",
                 __FILE__,
                 __LINE__,
                 __FUNCTION__);

        // console_log_warn("Vulkan2Showcase: Vulkan swapchain renderpass
        // Initialized Completed!!!");
        console_log_warn_tagged(
          "vulkan", "Vulkan swapchain renderpass Initialized Completed!!!");

        //! @note Initializing Depth Formats.
        // console_log_info("Vulkan2Showcase: Begin Vulkan swapchain Depth
        // Images for swapchains Initializing...");
        console_log_warn_tagged(
          "vulkan",
          "Begin Vulkan swapchain Depth Images for swapchains Initializing");
        VkFormat selected_depth_format = select_depth_format(m_physical);
        VkExtent2D swapchain_extent = m_swapchain_extent;

        //! @note Making sure our depth images are the same as our swapchains
        //! amount of images this swapchain can accept.
        m_swapchain_depth_images.resize(get_images_size());

        // Creating our images for depth view to lay-over

        //! @note We want to set the depth images to this memory property flag.
        //! @note To specify memory allocation with this type for most efficient
        //! for specifically device accesses.

        VkPhysicalDeviceMemoryProperties memory_properties;
        vkGetPhysicalDeviceMemoryProperties(m_physical, &memory_properties);

        VkMemoryPropertyFlags properties_flag =
          VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
        for (size_t i = 0; i < m_swapchain_depth_images.size(); i++) {
            VkImageCreateInfo image_create_info = {
                .sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO,
                .pNext = nullptr,
                .flags = 0,
                .format = selected_depth_format,
                .extent = { .width = swapchain_extent.width,
                            .height = swapchain_extent.height,
                            .depth = 1 },
                .mipLevels = 1,
                .arrayLayers = 1,
                .samples = VK_SAMPLE_COUNT_1_BIT,
                .tiling = VK_IMAGE_TILING_OPTIMAL,
                .usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT,
                .sharingMode = VK_SHARING_MODE_EXCLUSIVE,
                .initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
            };

            vk_check(vkCreateImage(m_driver,
                                   &image_create_info,
                                   nullptr,
                                   &m_swapchain_depth_images[i].Image),
                     "vkCreateImage",
                     __FILE__,
                     __LINE__,
                     __FUNCTION__);

            VkMemoryRequirements memory_requirements;
            vkGetImageMemoryRequirements(m_driver,
                                         m_swapchain_depth_images[i].Image,
                                         &memory_requirements);

            VkMemoryAllocateInfo mem_alloc_info = {
                .sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
                .allocationSize = memory_requirements.size,
                .memoryTypeIndex =
                  select_memory_type(memory_properties,
                                     memory_requirements.memoryTypeBits,
                                     properties_flag)
            };

            vk_check(
              vkAllocateMemory(m_driver,
                               &mem_alloc_info,
                               nullptr,
                               &m_swapchain_depth_images[i].DeviceMemory),
              "vkAllocateMemory",
              __FILE__,
              __LINE__,
              __FUNCTION__);

            vk_check(vkBindImageMemory(m_driver,
                                       m_swapchain_depth_images[i].Image,
                                       m_swapchain_depth_images[i].DeviceMemory,
                                       0),
                     "vkBindImageMemory",
                     __FILE__,
                     __LINE__,
                     __FUNCTION__);

            VkImageViewCreateInfo image_view_create_info = {
                .sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
                .image = m_swapchain_depth_images[i].Image,
                .viewType = VK_IMAGE_VIEW_TYPE_2D,
                .format = selected_depth_format,
                .subresourceRange = { .aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT,
                                      .baseMipLevel = 0,
                                      .levelCount = 1,
                                      .baseArrayLayer = 0,
                                      .layerCount = 0 }
            };

            vk_check(vkCreateImageView(m_driver,
                                       &image_view_create_info,
                                       nullptr,
                                       &m_swapchain_depth_images[i].ImageView),
                     "vkCreateImageView",
                     __FILE__,
                     __LINE__,
                     __FUNCTION__);
        }

        // console_log_warn("Vulkan2Showcase: Depth Images Initialized
        // Completed!!");
        console_log_warn_tagged("vulkan",
                                "Depth Images Initialized Completed!!");

        //! @note Setting up framebuffers for swapchain.
        // console_log_info("Vulkan2Showcase: Begin Framebuffers
        // Initialization!");
        console_log_warn_tagged("vulkan", "Begin Framebuffers Initialization!");
        m_swapchain_framebuffers.resize(get_images_size());

        for (size_t i = 0; i < get_images_size(); i++) {
            std::array<VkImageView, 2> image_attachment = {
                m_swapchain_images[i].ImageView,
                m_swapchain_depth_images[i].ImageView
            };

            VkExtent2D swapchain_image_extent = m_swapchain_extent;
            VkFramebufferCreateInfo fb_create_info = {
                .sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO,
                .pNext = nullptr,
                .flags = 0,
                .renderPass = m_swapchain_renderpass,
                .attachmentCount =
                  static_cast<uint32_t>(image_attachment.size()),
                .pAttachments = image_attachment.data(),
                .width = swapchain_image_extent.width,
                .height = swapchain_image_extent.height,
                .layers = 1
            };

            vk_check(vkCreateFramebuffer(m_driver,
                                         &fb_create_info,
                                         nullptr,
                                         &m_swapchain_framebuffers[i]),
                     "vkCreateFramebuffer",
                     __FILE__,
                     __LINE__,
                     __FUNCTION__);
        }

        // console_log_warn("Vulkan2Showcase: Vulkan Framebuffers Initialization
        // Completed!!!!");
        console_log_warn_tagged(
          "vulkan", "Vulkan Framebuffers Initialization Completed!!!!");

        //! @note Setting up Syncrhonization Objects for semaphores/fences.
        // console_log_info("Vulkan2Showcase: Begin Vulkan Sync Objects
        // Initialization");
        console_log_warn_tagged("vulkan",
                                "Begin Vulkan Sync Objects Initialization");

        //! @note Setting up our arrays
        // m_semaphores_images_available.resize(MaxFramesInFlight);
        m_semaphores_images_available.resize(max_frames_in_flight);
        // m_semaphores_render_completed.resize(max_frames_in_flight);
        m_semaphores_render_completed.resize(max_frames_in_flight);
        // m_swapchain_in_flight_fences.resize(max_frames_in_flight);
        m_swapchain_in_flight_fences.resize(max_frames_in_flight);
        // m_swapchain_fences.resize(get_images_size(), VK_NULL_HANDLE);
        m_swapchain_fences.resize(get_images_size(), VK_NULL_HANDLE);

        VkSemaphoreCreateInfo semaphore_create_info = {
            .sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO
        };

        VkFenceCreateInfo fence_create_info = {
            .sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO,
            .flags = VK_FENCE_CREATE_SIGNALED_BIT
        };

        for (size_t i = 0; i < max_frames_in_flight; i++) {
            vk_check(vkCreateSemaphore(m_driver,
                                       &semaphore_create_info,
                                       nullptr,
                                       &m_semaphores_images_available[i]),
                     "vkCreateSemaphore (1)",
                     __FILE__,
                     __LINE__,
                     __FUNCTION__);

            vk_check(vkCreateSemaphore(m_driver,
                                       &semaphore_create_info,
                                       nullptr,
                                       &m_semaphores_render_completed[i]),
                     "vkCreateSemaphore (2)",
                     __FILE__,
                     __LINE__,
                     __FUNCTION__);

            vk_check(vkCreateFence(m_driver,
                                   &fence_create_info,
                                   nullptr,
                                   &m_swapchain_in_flight_fences[i]),
                     "vkCreateFence",
                     __FILE__,
                     __LINE__,
                     __FUNCTION__);
        }

        // console_log_warn("Vulkan swapchain Sync Objects Initialized
        // Completed!!!!");
        console_log_warn_tagged(
          "vulkan", "swapchain Sync Objects Initialized Completed!!!!");

        // console_log_warn("Vulkan swapchain Initialized Complete");
        console_log_warn_tagged("vulkan", "swapchain Initialized Complete");
        m_is_resized_requested = false;
    }

    void vk_swapchain::recreate_swapchain(uint32_t p_width, uint32_t p_height) {
        on_create(p_width, p_height);
    }

    VkSwapchainKHR vk_swapchain::vk_swapchain_handler() {
        return m_swapchain;
    }

    VkFormat& vk_swapchain::read_swapchain_format() {
        return m_surface_format.format;
    }

    uint32_t vk_swapchain::images_size() const {
        return m_swapchain_images.size();
    }

    VkFramebuffer vk_swapchain::read_framebuffer(uint32_t index) {
        return m_swapchain_framebuffers[index];
    }

    VkImageView vk_swapchain::read_image_view(uint32_t index) {
        return m_swapchain_images[index].ImageView;
    }

    VkExtent2D vk_swapchain::read_swapchain_extent() {
        return m_swapchain_extent;
    }

    VkRenderPass vk_swapchain::read_swapchain_renderpass() {
        return m_swapchain_renderpass;
    }

    void vk_swapchain::submit_and_write_command_buffer(
      VkCommandBuffer* p_command_buffers) {
        // if(m_IsSwapchainResized){
        //     return;
        // }

        if (m_swapchain_fences[m_current_image_index] != VK_NULL_HANDLE) {
            vkWaitForFences(m_driver,
                            1,
                            &m_swapchain_fences[m_current_image_index],
                            true,
                            std::numeric_limits<uint64_t>::max());
        }

        m_swapchain_fences[m_current_image_index] =
          m_swapchain_in_flight_fences[m_current_frame_index];

        VkSubmitInfo submit_info = {
            .sType = VK_STRUCTURE_TYPE_SUBMIT_INFO,
            .pNext = nullptr,
        };

        VkSemaphore wait_semaphore[] = {
            m_semaphores_images_available[m_current_frame_index]
        };

        VkPipelineStageFlags wait_stages[] = {
            VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT
        };

        submit_info.waitSemaphoreCount = 1;
        submit_info.pWaitSemaphores = wait_semaphore;
        submit_info.pWaitDstStageMask = wait_stages;

        submit_info.commandBufferCount = 1;
        submit_info.pCommandBuffers = p_command_buffers;

        VkSemaphore signal_sems[] = {
            m_semaphores_render_completed[m_current_frame_index]
        };

        submit_info.signalSemaphoreCount = 1;
        submit_info.pSignalSemaphores = signal_sems;

        vkResetFences(
          m_driver, 1, &m_swapchain_in_flight_fences[m_current_frame_index]);

        vk_check(
          vkQueueSubmit(m_driver.get_graphics_queue(),
                        1,
                        &submit_info,
                        m_swapchain_in_flight_fences[m_current_frame_index]),
          "vkQueueSubmit",
          __FILE__,
          __LINE__,
          __FUNCTION__);

        //! @note Now we present our data to the display.
        VkPresentInfoKHR present_info = {
            .sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR,
            .waitSemaphoreCount = 1,
            .pWaitSemaphores = signal_sems,
        };

        VkSwapchainKHR swapchains_array[] = { m_swapchain };
        present_info.swapchainCount = 1;
        present_info.pSwapchains = swapchains_array;

        present_info.pImageIndices = &m_current_image_index;
        if (m_presentation_queue == VK_NULL_HANDLE) {
            console_log_error("PresentationQueue is nullptr!!!");
        }

        auto res = vkQueuePresentKHR(m_presentation_queue, &present_info);
        if (res == VK_ERROR_OUT_OF_DATE_KHR) {
            m_is_resized_requested = true;
            g_is_swapchain_resized = true;
            console_log_error("m_IsSwapchainResized = {}",
                              m_is_resized_requested);
            on_create(application::get_window().get_width(),
                      application::get_window().get_height());
            return;
        }

        vk_check(res, "vkQueuePresentKHR", __FILE__, __LINE__, __FUNCTION__);

        m_current_frame_index =
          (m_current_frame_index + 1) % vk_swapchain::max_frames_in_flight;
    }

    uint32_t vk_swapchain::read_acquire_next_frame() {
        uint32_t image_index;
        vk_check(
          vkWaitForFences(m_driver,
                          1,
                          &m_swapchain_in_flight_fences[m_current_frame_index],
                          true,
                          std::numeric_limits<uint32_t>::max()),
          "vkWaitForFences",
          __FILE__,
          __LINE__,
          __FUNCTION__);

        auto res = vkAcquireNextImageKHR(
          m_driver,
          m_swapchain,
          std::numeric_limits<uint64_t>::max(),
          m_semaphores_images_available[m_current_frame_index],
          VK_NULL_HANDLE,
          &image_index);
        if (res == VK_ERROR_OUT_OF_DATE_KHR || res == VK_SUBOPTIMAL_KHR) {
            m_is_resized_requested = true;
            on_create(application::get_window().get_width(),
                      application::get_window().get_height());
            return -3;
        }

        vk_check(
          res, "vkAcquireNextImageKHR", __FILE__, __LINE__, __FUNCTION__);
        m_current_image_index = image_index;
        return image_index;
    }

    bool vk_swapchain::swapchain_resized() {
        return m_is_resized_requested;
    }

    void vk_swapchain::resize_status(bool is_resize_signaled) {
        m_is_resized_requested = is_resize_signaled;
    }

    uint32_t vk_swapchain::current_frame_per_tick() {
        return m_current_frame_index;
    }

    uint32_t vk_swapchain::select_memory_type(
      VkPhysicalDeviceMemoryProperties p_memory_properties,
      uint32_t p_type_filter,
      VkMemoryPropertyFlags property_flag) {
        for (uint32_t i = 0; i < p_memory_properties.memoryTypeCount; i++) {
            if ((p_type_filter & (1 << i)) &&
                (p_memory_properties.memoryTypes[i].propertyFlags &
                 property_flag) == property_flag) {
                return i;
            }
        }
        console_log_error("vk_swapchain::select_memory_type returns zero "
                          "meaning failed to find suitable memory type");
        return 0;
    }

    VkPresentModeKHR vk_swapchain::select_compatible_present_mode(
      const VkPresentModeKHR& p_request_mode,
      const std::vector<VkPresentModeKHR>& p_modes) {
        for (const auto& mode : p_modes) {
            if (mode == p_request_mode) {
                return mode;
            }
        }
        return VK_PRESENT_MODE_FIFO_KHR;
    }

    VkExtent2D vk_swapchain::select_valid_extent(
      const VkSurfaceCapabilitiesKHR& p_surface_capabilities) {
        //! @note Width/Height of our current swapchain

        //! @note All this checks is the width/height are not set, then whatever
        //! the swapchain is set to will set the extent by default
        if (p_surface_capabilities.currentExtent.width == (uint32_t)-1) {
            m_swapchain_extent.width = m_width;
            m_swapchain_extent.height = m_height;
        }
        else {
            m_swapchain_extent = p_surface_capabilities.currentExtent;
            m_width = p_surface_capabilities.currentExtent.width;
            m_height = p_surface_capabilities.currentExtent.height;
        }

        return p_surface_capabilities.currentExtent;
    }

    VkFormat vk_swapchain::select_depth_format(
      const VkPhysicalDevice& p_physical_device) {
        return select_supported_format(
          p_physical_device,
          { VK_FORMAT_D32_SFLOAT,
            VK_FORMAT_D32_SFLOAT_S8_UINT,
            VK_FORMAT_D24_UNORM_S8_UINT },
          VK_IMAGE_TILING_OPTIMAL,
          VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT);
    }

    VkFormat vk_swapchain::select_supported_format(
      VkPhysicalDevice p_physical,
      const std::vector<VkFormat>& p_formats,
      VkImageTiling p_tiling,
      VkFormatFeatureFlags p_feature_flags) {
        VkFormat valid_format = VK_FORMAT_UNDEFINED;

        for (const auto& format : p_formats) {
            VkFormatProperties properties;
            vkGetPhysicalDeviceFormatProperties(
              p_physical, format, &properties);

            // if ((p_tiling == VK_IMAGE_TILING_LINEAR ||
            //      p_tiling == VK_IMAGE_TILING_OPTIMAL) &&
            //     (properties.linearTilingFeatures & p_feature_flags) ==
            //       p_feature_flags) {
            //     valid_format = format;
            // }

            if (p_tiling == VK_IMAGE_TILING_LINEAR &&
                (properties.linearTilingFeatures & p_feature_flags) ==
                  p_feature_flags) {
                valid_format = format;
            }

            if (p_tiling == VK_IMAGE_TILING_OPTIMAL &&
                (properties.optimalTilingFeatures & p_feature_flags) ==
                  p_feature_flags) {
                valid_format = format;
            }

            // switch (p_tiling){
            // case VK_IMAGE_TILING_LINEAR:
            // 	if(properties.linearTilingFeatures & p_feature_flags) {
            // 		valid_format = format;
            // 	}
            // 	break;
            // case VK_IMAGE_TILING_OPTIMAL:
            // 	if(properties.optimalTilingFeatures & p_feature_flags) {
            // 		valid_format = format;
            // 	}
            // 	break;
            // default:
            // 	break;
            // }
        }

        vk_check_format(valid_format, __FILE__, __LINE__, __FUNCTION__);

        return valid_format;
    }

    bool vk_swapchain::is_resized() {
        return g_is_swapchain_resized;
    }

    void vk_swapchain::resize_reset() {
        g_is_swapchain_resized = false;
    }

};