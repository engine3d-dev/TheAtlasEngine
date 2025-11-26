#include <drivers/vulkan-cpp/vk_swapchain.hpp>
#include <drivers/vulkan-cpp/vk_context.hpp>
#include <core/engine_logger.hpp>
#include <drivers/vulkan-cpp/utilities.hpp>
#include <array>
#include <vulkan-cpp/types.hpp>

namespace atlas::vk {

    vk_swapchain::vk_swapchain(const VkSurfaceKHR& p_surface,
                               const window_settings& p_settings)
      : m_current_surface_handler(p_surface)
      , m_window_settings(p_settings)
      , m_current_surface(p_surface) {
        m_physical = vk_context::physical_driver();
        m_driver = vk_context::driver_context();

        create();
    }

    void vk_swapchain::create() {

        // surface properties are always updated from the physical device
        // so they are valid should the swapchain be recreated
        m_surface_properties =
          m_physical.get_surface_properties(m_current_surface);
        m_window_settings.width =
          m_surface_properties.surface_capabilities.currentExtent.width;
        m_window_settings.height =
          m_surface_properties.surface_capabilities.currentExtent.height;

        //! @note Setting up presentation stuff
        // request what our minimum image count is
        uint32_t request_min_image_count =
          select_images_size(m_surface_properties.surface_capabilities);

        m_swapchain_extent =
          m_surface_properties.surface_capabilities.currentExtent;

        // setting our presentation properties
        uint32_t present_index =
          m_physical.read_presentation_index(m_current_surface_handler);

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
            .pQueueFamilyIndices = &present_index,
            .preTransform =
              m_surface_properties.surface_capabilities.currentTransform,
            .compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
            .presentMode = VK_PRESENT_MODE_IMMEDIATE_KHR,
            .clipped = true
        };

        vk_check(vkCreateSwapchainKHR(
                   m_driver, &swapchain_ci, nullptr, &m_swapchain_handler),
                 "vkCreateSwapchainKHR");

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
        m_swapchain_images.resize(image_count);
        m_image_size = image_count;
        m_swapchain_depth_images.resize(image_count);

        VkFormat depth_format = m_driver.depth_format();

        for (uint32_t i = 0; i < m_swapchain_images.size(); i++) {
            ::vk::image_params color_image_config = {
                .extent = { m_swapchain_extent.width,
                            m_swapchain_extent.height, },
                .format = m_surface_properties.surface_format.format,
                .aspect = ::vk::image_aspect_flags::color_bit,
                .usage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
                .mip_levels = 1,
                .layer_count = 1,
                .phsyical_memory_properties = m_physical.memory_properties(),

            };

            m_swapchain_images[i] =
              ::vk::sample_image(m_driver, images[i], color_image_config);

            ::vk::image_params depth_image_config = {
                .extent = { m_swapchain_extent.width,
                            m_swapchain_extent.height, },
                .format = depth_format,
                .aspect = ::vk::image_aspect_flags::depth_bit,
                .usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT,
                .mip_levels = 1,
                .layer_count = 1,
                .phsyical_memory_properties = m_physical.memory_properties(),
            };

            m_swapchain_depth_images[i] =
              ::vk::sample_image(m_driver, depth_image_config);
        }

        // command buffers

        m_swapchain_command_buffers.resize(image_count);

        for (size_t i = 0; i < m_swapchain_command_buffers.size(); i++) {
            ::vk::command_params settings = {
                .levels = ::vk::command_levels::primary,
                // .queue_index = enumerate_swapchain_settings.present_index,
                .queue_index = 0,
                .flags = ::vk::command_pool_flags::reset,
            };

            m_swapchain_command_buffers[i] =
              ::vk::command_buffer(m_driver, settings);
        }

        std::array<::vk::attachment, 2> renderpass_attachments = {
            ::vk::attachment{
              .format = m_surface_properties.surface_format.format,
              .layout = ::vk::image_layout::color_optimal,
              .samples = ::vk::sample_bit::count_1,
              .load = ::vk::attachment_load::clear,
              .store = ::vk::attachment_store::store,
              .stencil_load = ::vk::attachment_load::clear,
              .stencil_store = ::vk::attachment_store::dont_care,
              .initial_layout = ::vk::image_layout::undefined,
              .final_layout = ::vk::image_layout::present_src_khr,
            },
            ::vk::attachment{
              .format = depth_format,
              .layout = ::vk::image_layout::depth_stencil_optimal,
              .samples = ::vk::sample_bit::count_1,
              .load = ::vk::attachment_load::clear,
              .store = ::vk::attachment_store::dont_care,
              .stencil_load = ::vk::attachment_load::dont_care,
              .stencil_store = ::vk::attachment_store::dont_care,
              .initial_layout = ::vk::image_layout::undefined,
              .final_layout = ::vk::image_layout::depth_stencil_optimal,
            },
        };
        m_final_renderpass = ::vk::renderpass(m_driver, renderpass_attachments);

        // creating framebuffers
        m_swapchain_framebuffers.resize(m_swapchain_images.size());

        for (uint32_t i = 0; i < m_swapchain_images.size(); i++) {

            std::array<VkImageView, renderpass_attachments.size()>
              image_attachments = { m_swapchain_images[i].image_view(),
                                    m_swapchain_depth_images[i].image_view() };

            ::vk::framebuffer_params framebuffer_info = {
                .renderpass = m_final_renderpass,
                .views = image_attachments,
                .extent = m_swapchain_extent
            };
            m_swapchain_framebuffers[i] =
              ::vk::framebuffer(m_driver, framebuffer_info);
        }

        ::vk::queue_params present_queue_params{
            .family = 0,
            .index = 0,
        };
        m_present_to_queue = ::vk::device_present_queue(
          m_driver, m_swapchain_handler, present_queue_params);
    }

    void vk_swapchain::invalidate() {
        destroy();
        create();
    }

    uint32_t vk_swapchain::select_images_size(
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

    uint32_t vk_swapchain::read_acquired_image() {
        m_present_to_queue.wait_idle();

        // uint32_t frame_idx = m_present_to_queue.acquired_frame();
        uint32_t frame_idx = m_present_to_queue.acquire_next_image();
        if (m_present_to_queue.out_of_date()) {
            invalidate();
            m_present_to_queue.out_of_date(false);
            frame_idx = m_present_to_queue.acquire_next_image();
        }

        return frame_idx;
    }

    void vk_swapchain::present(const uint32_t& p_current_frame) {
        m_present_to_queue.present_frame(p_current_frame);
        if (m_present_to_queue.out_of_date()) {
            invalidate();
            m_present_to_queue.out_of_date(false);
        }
    }

    void vk_swapchain::submit(std::span<const VkCommandBuffer> p_commands) {
        // m_present_to_queue.submit_immediate_async(p_command);
        m_present_to_queue.submit_async(p_commands);
    }

    void vk_swapchain::destroy() {
        vkDeviceWaitIdle(m_driver);

        for (size_t i = 0; i < m_swapchain_framebuffers.size(); i++) {
            m_swapchain_framebuffers[i].destroy();
        }

        m_final_renderpass.destroy();

        m_present_to_queue.destroy();

        for (size_t i = 0; i < m_swapchain_command_buffers.size(); i++) {
            m_swapchain_command_buffers[i].destroy();
        }

        for (uint32_t i = 0; i < m_swapchain_depth_images.size(); i++) {
            m_swapchain_depth_images[i].destroy();
        }

        for (uint32_t i = 0; i < m_swapchain_images.size(); i++) {
            m_swapchain_images[i].destroy();
        }

        vkDestroySwapchainKHR(m_driver, m_swapchain_handler, nullptr);
    }
};
