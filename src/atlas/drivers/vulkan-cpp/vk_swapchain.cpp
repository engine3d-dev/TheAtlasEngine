#include <drivers/vulkan-cpp/vk_swapchain.hpp>
#include <drivers/vulkan-cpp/vk_context.hpp>
#include <core/engine_logger.hpp>
#include <drivers/vulkan/helper_functions.hpp>
#include <drivers/vulkan-cpp/vk_types.hpp>
#include <array>

namespace atlas::vk {
    VkPresentModeKHR select_compatible_present_mode(
      const VkPresentModeKHR& p_request,
      const std::vector<VkPresentModeKHR>& p_modes) {
        for (const auto& mode : p_modes) {
            if (mode == p_request) {
                return mode;
            }
        }
        return VK_PRESENT_MODE_FIFO_KHR;
    }

    // validate the capabilities to ensure we are not requesting the maximum
    // over the amount of images we are able to request
    static uint32_t select_images_size(
      const VkSurfaceCapabilitiesKHR& p_surface_capabilities) {
        uint32_t requested_images = p_surface_capabilities.minImageCount + 1;

        uint32_t final_image_count = 0;

        if ((p_surface_capabilities.maxImageCount > 0) and
            (requested_images > p_surface_capabilities.maxImageCount)) {
            final_image_count = p_surface_capabilities.maxImageCount;
        }
        else {
            final_image_count = requested_images;
        }

        return final_image_count;
    }


    vk_swapchain::vk_swapchain(const VkSurfaceKHR& p_surface,
                               const window_settings& p_settings)
      : m_current_surface_handler(p_surface)
      , m_window_settings(p_settings),
	  m_current_surface(p_surface) {
        console_log_trace("vk_swapchain begin initialization!!!");
        m_physical = vk_context::physical_driver();
        m_driver = vk_context::driver_context();
		m_surface_properties = m_physical.get_surface_properties(m_current_surface);

		on_create();
    }

	void vk_swapchain::on_create() {

        //! @note Setting up presentation stuff
        // request what our minimum image count is
        uint32_t request_min_image_count =
          select_images_size(m_surface_properties.surface_capabilities);

        m_swapchain_extent =
          m_surface_properties.surface_capabilities.currentExtent;

        // setting our presentation properties
        uint32_t present_index =
          m_physical.read_presentation_index(m_current_surface_handler);
        console_log_trace("Presentation Index = {}", present_index);
        vk_queue_options present_properties = { .family_index = present_index };

        VkSwapchainCreateInfoKHR swapchain_ci = {
            .sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
            .surface = m_current_surface_handler,
            .minImageCount = request_min_image_count,
            .imageFormat = m_surface_properties.surface_format.format,
            .imageColorSpace = m_surface_properties.surface_format.colorSpace,
            // use physical device surface formats to getting the right formats
            // in vulkan
            .imageExtent = m_swapchain_extent,
            .imageArrayLayers = 1,
            .imageUsage = (VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT |
                           VK_IMAGE_USAGE_TRANSFER_DST_BIT),
            .queueFamilyIndexCount = 1,
            .pQueueFamilyIndices = &present_properties.family_index,
            .preTransform =
              m_surface_properties.surface_capabilities.currentTransform,
            .compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
            .presentMode = VK_PRESENT_MODE_IMMEDIATE_KHR,
            .clipped = true
        };

        vk_check(vkCreateSwapchainKHR(
                   m_driver, &swapchain_ci, nullptr, &m_swapchain_handler),
                 "vkCreateSwapchainKHR",
                 __FILE__,
                 __LINE__,
                 __FUNCTION__);

        //!@brief querying images available
        uint32_t image_count = 0;
        vkGetSwapchainImagesKHR(m_driver,
                                m_swapchain_handler,
                                &image_count,
                                nullptr); // used to get the amount of images
        std::vector<VkImage> images(image_count);
        vkGetSwapchainImagesKHR(m_driver,
                                m_swapchain_handler,
                                &image_count,
                                images.data()); // used to store in the images

        // Creating Images
        console_log_info("vk_swapchain begin images initialization!!!");
        m_swapchain_images.resize(image_count);
        m_swapchain_depth_images.resize(image_count);

        console_log_info("swapchain images.size() = {}",
                         m_swapchain_images.size());
        VkFormat depth_format = m_driver.depth_format();
        uint32_t layer_count = 1;
        uint32_t mip_levels = 1;
        for (uint32_t i = 0; i < m_swapchain_images.size(); i++) {
            m_swapchain_images[i].Image = images[i];
            m_swapchain_images[i].ImageView =
              create_image_view(m_driver,
                                images[i],
                                m_surface_properties.surface_format,
                                VK_IMAGE_ASPECT_COLOR_BIT,
                                VK_IMAGE_VIEW_TYPE_2D,
                                layer_count,
                                mip_levels);

            // Creating Depth Images for depth buffering
            VkImageUsageFlagBits usage =
              VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
            VkMemoryPropertyFlagBits property_flags =
              VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
            // Creates our images
            m_swapchain_depth_images[i] =
              create_image2d(m_swapchain_extent.width,
                             m_swapchain_extent.height,
                             depth_format,
                             usage,
                             property_flags);
            m_swapchain_depth_images[i].ImageView =
              create_image_view(m_swapchain_depth_images[i].Image,
                                depth_format,
                                VK_IMAGE_ASPECT_DEPTH_BIT);
        }

        console_log_info("vk_swapchain end image initialization!!!");

        // command buffers
        console_log_info("vk_swapchain begin initializing command buffers!!!!");

        m_swapchain_command_buffers.resize(image_count);
        console_log_trace("command buffers.size() = {}",
                          m_swapchain_command_buffers.size());

        for (size_t i = 0; i < m_swapchain_command_buffers.size(); i++) {
            command_buffer_settings settings = {
                present_index,
                command_buffer_levels::Primary,
                VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT
            };

            m_swapchain_command_buffers[i] = vk_command_buffer(settings);
        }

        console_log_info(
          "vk_swapchain successfully initialized command buffers!!!!\n\n");

        // m_swapchain_renderpass = create_simple_renderpass(
        //   m_driver, m_surface_properties.surface_format);
		// m_swapchain_main_renderpass.configure(m_renderpass_options);
		VkAttachmentDescription color_attachment  = {
            .flags = 0,
            .format = m_surface_properties.surface_format.format,
            .samples = VK_SAMPLE_COUNT_1_BIT,
            .loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR,
            .storeOp = VK_ATTACHMENT_STORE_OP_STORE,
            .stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE,
            .stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE,
            .initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
            .finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR
        };

		VkAttachmentDescription depth_attachment = {
			.flags = 0,
            .format = m_driver.depth_format(),
            .samples = VK_SAMPLE_COUNT_1_BIT,
            .loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR,
            .storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE,
            .stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE,
            .stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE,
            .initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
            .finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL
		};

		std::array<VkAttachmentDescription, 2> attachments = {
			color_attachment,
			depth_attachment
		};

		VkAttachmentReference color_attachment_ref = {
            .attachment = 0, .layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL
        };

        VkAttachmentReference depth_attachment_reference = {
            .attachment = 1,
            .layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL
        };

        VkSubpassDescription subpass_description = {
            .flags = 0,
            .pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS,
            .inputAttachmentCount = 0,
            .pInputAttachments = nullptr,
            .colorAttachmentCount = 1,
            .pColorAttachments = &color_attachment_ref,
            .pResolveAttachments = nullptr,
            .pDepthStencilAttachment =
              &depth_attachment_reference, // enable depth buffering
            .preserveAttachmentCount = 0,
            .pPreserveAttachments = nullptr
        };

		std::array<VkSubpassDescription, 1> subpass_desc = {
			subpass_description
		};

		vk_renderpass_options renderpass_options = {
			.attachments = attachments,
			.subpass_descriptions = subpass_desc
		};

		m_swapchain_main_renderpass = vk_renderpass(renderpass_options);

        // creating framebuffers
		console_log_error("begin vk_swapchain initializing framebuffers!!");
        m_swapchain_framebuffers.resize(m_swapchain_images.size());

        for (uint32_t i = 0; i < m_swapchain_images.size(); i++) {
            std::vector<VkImageView> image_view_attachments;
            image_view_attachments.push_back(m_swapchain_images[i].ImageView);
            image_view_attachments.push_back(
              m_swapchain_depth_images[i].ImageView);

            VkFramebufferCreateInfo framebuffer_ci = {
                .sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO,
                .pNext = nullptr,
                .flags = 0,
                .renderPass = m_swapchain_main_renderpass,
                // .attachmentCount = 1,
                // .pAttachments = &m_swapchain_images[i].ImageView,
                .attachmentCount =
                  static_cast<uint32_t>(image_view_attachments.size()),
                .pAttachments = image_view_attachments.data(),
                .width = m_swapchain_extent.width,
                .height = m_swapchain_extent.height,
                .layers = 1
            };

            vk_check(vkCreateFramebuffer(m_driver,
                                         &framebuffer_ci,
                                         nullptr,
                                         &m_swapchain_framebuffers[i]),
                     "vkCreateFramebuffer",
                     __FILE__,
                     __LINE__,
                     __FUNCTION__);
		}

		console_log_error("end of vk_swapchain framebuffers initialization!!!\n\n");

		vk_queue_options options = {
			.family_index = 0, // using defauly queue family
			.queue_index = 0 // using defauly presentation queue available
		};
		m_present_to_queue = vk_present_queue(m_swapchain_handler, options);

        console_log_trace("vk_swapchain end initialization!!!");
		
	}

	void vk_swapchain::recreate() {
		console_log_trace("vk_swapchain recreation!!!");
		destroy();
		on_create();
	}

	uint32_t vk_swapchain::read_acquired_image() {
		m_present_to_queue.wait_idle();

		if(m_present_to_queue.resize_requested()) {
			console_log_fatal("presentation queue recreation requested!!!!");
			recreate();
			m_present_to_queue.set_resize_status(false);
			console_log_error("resize requesteed = {}", m_present_to_queue.resize_requested());
		}

		uint32_t frame_idx = m_present_to_queue.acquired_frame();
		return frame_idx;
	}

	void vk_swapchain::present(const uint32_t& p_current_frame) {
		if(m_present_to_queue.resize_requested()) {
			console_log_fatal("presentation queue recreation requested!!!!");
			recreate();
			m_present_to_queue.set_resize_status(false);
			console_log_error("resize requesteed = {}", m_present_to_queue.resize_requested());
		}
		m_present_to_queue.present_frame(p_current_frame);
	}

	void vk_swapchain::submit(const VkCommandBuffer& p_command) {
		m_present_to_queue.submit_immediate_async(p_command);
	}

	void vk_swapchain::destroy() {
		console_log_fatal("vk_swapchain::destroy()!!!");
		vkDeviceWaitIdle(m_driver);

		for (size_t i = 0; i < m_swapchain_framebuffers.size(); i++) {
			vkDestroyFramebuffer(
		m_driver, m_swapchain_framebuffers[i], nullptr);
		}

		// vkDestroyRenderPass(m_driver, m_swapchain_renderpass, nullptr);
		m_swapchain_main_renderpass.destroy();

		m_present_to_queue.destroy();

		for (size_t i = 0; i < m_swapchain_command_buffers.size(); i++) {
			m_swapchain_command_buffers[i].destroy();
		}

		for (uint32_t i = 0; i < m_swapchain_depth_images.size(); i++) {
			vkDestroyImageView(
		m_driver, m_swapchain_depth_images[i].ImageView, nullptr);

			vkDestroyImage(
		m_driver, m_swapchain_depth_images[i].Image, nullptr);

			vkFreeMemory(
		m_driver, m_swapchain_depth_images[i].DeviceMemory, nullptr);
		}

		for (uint32_t i = 0; i < m_swapchain_images.size(); i++) {
			vkDestroyImageView(
		m_driver, m_swapchain_images[i].ImageView, nullptr);
		}

		vkDestroySwapchainKHR(m_driver, m_swapchain_handler, nullptr);
	}
};