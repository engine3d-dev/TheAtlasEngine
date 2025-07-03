#include <drivers/vulkan-cpp/utilties/utils.hpp>
#include <drivers/vulkan-cpp/vk_context.hpp>
#include <drivers/vulkan-cpp/helper_functions.hpp>

namespace atlas::vk {

    VkCommandPool create_single_command_pool() {
        VkDevice driver = vk_context::driver_context();
        vk_physical_driver physical = vk_context::physical_driver();
        uint32_t graphics_queue_index =
          physical.read_queue_family_indices().graphics;
        VkCommandPoolCreateInfo pool_ci = {
            .sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
            .pNext = nullptr,
            .flags = 0,
            .queueFamilyIndex = graphics_queue_index
        };

        VkCommandPool command_pool = nullptr;
        vk_check(vkCreateCommandPool(driver, &pool_ci, nullptr, &command_pool),
                 "vkCreateCommandPool",
                 __FILE__,
                 __LINE__,
                 __FUNCTION__);

        return command_pool;
    }

    VkCommandBuffer create_single_command_buffer(
      const VkCommandPool& p_command_pool) {
        VkDevice driver = vk_context::driver_context();

        VkCommandBufferAllocateInfo command_buffer_alloc_info = {
            .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
            .pNext = nullptr,
            .commandPool = p_command_pool,
            .level = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
            .commandBufferCount = 1
        };

        VkCommandBuffer command_buffer = nullptr;
        vk_check(vkAllocateCommandBuffers(
                   driver, &command_buffer_alloc_info, &command_buffer),
                 "vkAllocateCommandBuffers",
                 __FILE__,
                 __LINE__,
                 __FUNCTION__);

        return command_buffer;
    }

    void begin_command_buffer(const VkCommandBuffer& p_command_buffer,
                              VkCommandBufferUsageFlags p_usage_flags) {
        VkCommandBufferBeginInfo command_buffer_begin_info = {
            .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
            .pNext = nullptr,
            .flags = p_usage_flags,
            .pInheritanceInfo = nullptr
        };

        vk_check(
          vkBeginCommandBuffer(p_command_buffer, &command_buffer_begin_info),
          "vkBeginCommandBuffer",
          __FILE__,
          __LINE__,
          __FUNCTION__);
    }

    void end_command_buffer(const VkCommandBuffer& p_command_buffer) {
        vkEndCommandBuffer(p_command_buffer);
    }

    void copy(vk_command_buffer& p_command_buffer,
              VkImage& p_image,
              VkBuffer& p_buffer,
              uint32_t p_width,
              uint32_t p_height) {
        VkQueue graphics_queue = vk_context::driver_context().graphics_queue();
        p_command_buffer.begin(VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT);

        VkBufferImageCopy buffer_image_copy = {
            .bufferOffset = 0,
            .bufferRowLength = 0,
            .bufferImageHeight = 0,
            .imageSubresource = { .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
                                  .mipLevel = 0,
                                  .baseArrayLayer = 0,
                                  .layerCount = 1 },
            .imageOffset = { .x = 0, .y = 0, .z = 0 },
            .imageExtent = { .width = p_width, .height = p_height, .depth = 1 }
        };

        vkCmdCopyBufferToImage(p_command_buffer,
                               p_buffer,
                               p_image,
                               VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                               1,
                               &buffer_image_copy);

        p_command_buffer.end();

        VkCommandBuffer buffer = p_command_buffer;

        VkSubmitInfo submit_info = {
            .sType = VK_STRUCTURE_TYPE_SUBMIT_INFO,
            .commandBufferCount = 1,
            .pCommandBuffers = &buffer,
        };

        vkQueueSubmit(graphics_queue, 1, &submit_info, nullptr);
        vkQueueWaitIdle(graphics_queue);
    }

    void copy(const vk_buffer& p_src,
              const vk_buffer& p_dst,
              uint32_t p_size_of_bytes) {
        vk_driver driver = vk_context::driver_context();
        VkQueue graphics_queue = driver.graphics_queue();
        VkCommandPool command_pool = create_single_command_pool();
        VkCommandBuffer copy_cmd_buffer =
          create_single_command_buffer(command_pool);

        begin_command_buffer(copy_cmd_buffer,
                             VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT);
        VkBufferCopy copy_region{};
        copy_region.size = (VkDeviceSize)p_size_of_bytes;
        vkCmdCopyBuffer(
          copy_cmd_buffer, p_src.handler, p_dst.handler, 1, &copy_region);
        end_command_buffer(copy_cmd_buffer);

        VkSubmitInfo submit_info{};
        submit_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        submit_info.commandBufferCount = 1;
        submit_info.pCommandBuffers = &copy_cmd_buffer;

        vkQueueSubmit(graphics_queue, 1, &submit_info, nullptr);
        vkQueueWaitIdle(graphics_queue);

        vkFreeCommandBuffers(driver, command_pool, 1, &copy_cmd_buffer);
        vkDestroyCommandPool(driver, command_pool, nullptr);
    }

    void copy(const vk_buffer& p_src,
              const vk_buffer& p_dst,
              size_t p_size_of_bytes) {
        VkDevice driver = vk_context::driver_context();
        VkQueue graphics_queue = vk_context::driver_context().graphics_queue();
        VkCommandPool command_pool = create_single_command_pool();
        VkCommandBuffer copy_cmd_buffer =
          create_single_command_buffer(command_pool);

        begin_command_buffer(copy_cmd_buffer,
                             VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT);

        VkBufferCopy copy_region = { .size = p_size_of_bytes };

        vkCmdCopyBuffer(
          copy_cmd_buffer, p_src.handler, p_dst.handler, 1, &copy_region);
        end_command_buffer(copy_cmd_buffer);

        VkSubmitInfo submit_info{};
        submit_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        submit_info.commandBufferCount = 1;
        submit_info.pCommandBuffers = &copy_cmd_buffer;
        vkQueueSubmit(graphics_queue, 1, &submit_info, nullptr);
        vkQueueWaitIdle(graphics_queue);

        vkFreeCommandBuffers(driver, command_pool, 1, &copy_cmd_buffer);
        vkDestroyCommandPool(driver, command_pool, nullptr);
    }

    void write(const vk_buffer& p_buffer,
               const void* p_data,
               size_t p_size_in_bytes) {
        VkDevice driver = vk_context::driver_context();
        void* mapped = nullptr;
        vk_check(
          vkMapMemory(
            driver, p_buffer.device_memory, 0, p_size_in_bytes, 0, &mapped),
          "vkMapMemory",
          __FILE__,
          __LINE__,
          __FUNCTION__);
        memcpy(mapped, p_data, p_size_in_bytes);
        vkUnmapMemory(driver, p_buffer.device_memory);
    }

    //
    void write(const vk_buffer& p_buffer,
               const std::span<vertex_input>& p_in_buffer) {
        VkDeviceSize buffer_size =
          p_in_buffer
            .size_bytes(); // does equivalent to doing sizeof(p_in_buffer[0]) *
                           // p_in_buffer.size();
        VkDevice driver = vk_context::driver_context();
        void* mapped = nullptr;
        vk_check(vkMapMemory(
                   driver, p_buffer.device_memory, 0, buffer_size, 0, &mapped),
                 "vkMapMemory",
                 __FILE__,
                 __LINE__,
                 __FUNCTION__);
        memcpy(mapped, p_in_buffer.data(), buffer_size);
        vkUnmapMemory(driver, p_buffer.device_memory);
    }

    void write(const vk_buffer& p_buffer,
               const std::span<uint32_t>& p_in_buffer) {
        VkDeviceSize buffer_size = p_in_buffer.size_bytes();
        VkDevice driver = vk_context::driver_context();
        void* mapped = nullptr;
        vk_check(vkMapMemory(
                   driver, p_buffer.device_memory, 0, buffer_size, 0, &mapped),
                 "vkMapMemory",
                 __FILE__,
                 __LINE__,
                 __FUNCTION__);
        memcpy(mapped, p_in_buffer.data(), buffer_size);
        vkUnmapMemory(driver, p_buffer.device_memory);
    }

    void transition_image_layout(VkImage& p_image,
                                 VkFormat p_format,
                                 VkImageLayout p_old,
                                 VkImageLayout p_new) {
        VkQueue graphics_queue = vk_context::driver_context().graphics_queue();
        command_buffer_settings properties = {
            0,
            command_buffer_levels::primary,
            VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT,
        };

        vk_command_buffer temp_copy_command_buffer =
          vk_command_buffer(properties);

        temp_copy_command_buffer.begin(
          VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT);

        // 1. image memory barrier
        VkCommandBuffer handle = temp_copy_command_buffer;
        image_memory_barrier(handle, p_image, p_format, p_old, p_new);

        // submit one copy
        temp_copy_command_buffer.end();

        VkCommandBuffer buffer = temp_copy_command_buffer;

        VkSubmitInfo submit_info = {
            .sType = VK_STRUCTURE_TYPE_SUBMIT_INFO,
            .commandBufferCount = 1,
            .pCommandBuffers = &buffer,
        };

        vkQueueSubmit(graphics_queue, 1, &submit_info, VK_NULL_HANDLE);
        vkQueueWaitIdle(graphics_queue);

        temp_copy_command_buffer.destroy();
    }

	void transition_image_layout(VkCommandBuffer& p_command_buffer,
                                 VkImage& p_image,
                                 VkFormat p_format,
                                 VkImageLayout p_old,
                                 VkImageLayout p_new) {

        image_memory_barrier(p_command_buffer, p_image, p_format, p_old, p_new);

	}

    void image_memory_barrier(VkCommandBuffer& p_command_buffer,
                              VkImage& p_image,
                              VkFormat p_format,
                              VkImageLayout p_old,
                              VkImageLayout p_new) {
        VkImageMemoryBarrier image_memory_barrier = {
            .sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
            .pNext = nullptr,
            .srcAccessMask = 0,
            .dstAccessMask = 0,
            .oldLayout = p_old,
            .newLayout = p_new,
            .srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
            .dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
            .image = p_image,
            .subresourceRange = { .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
                                  .baseMipLevel = 0,
                                  .levelCount = 1,
                                  .baseArrayLayer = 0,
                                  .layerCount = 1 }
        };

        VkPipelineStageFlags source_stage;
        VkPipelineStageFlags dst_stages;

        if (p_new == VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL ||
            (p_format == VK_FORMAT_D16_UNORM) ||
            (p_format == VK_FORMAT_X8_D24_UNORM_PACK32) ||
            (p_format == VK_FORMAT_D32_SFLOAT) ||
            (p_format == VK_FORMAT_S8_UINT) ||
            (p_format == VK_FORMAT_D16_UNORM_S8_UINT) ||
            (p_format == VK_FORMAT_D24_UNORM_S8_UINT)) {
            image_memory_barrier.subresourceRange.aspectMask =
              VK_IMAGE_ASPECT_DEPTH_BIT;

            if (has_stencil_attachment(p_format)) {
                image_memory_barrier.subresourceRange.aspectMask |=
                  VK_IMAGE_ASPECT_STENCIL_BIT;
            }
        }
        else {
            image_memory_barrier.subresourceRange.aspectMask =
              VK_IMAGE_ASPECT_COLOR_BIT;
        }

        if (p_old == VK_IMAGE_LAYOUT_UNDEFINED &&
            p_new == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL) {
            image_memory_barrier.srcAccessMask = 0;
            image_memory_barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

            source_stage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
            dst_stages = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
        }
        else if (p_old == VK_IMAGE_LAYOUT_UNDEFINED &&
                 p_new == VK_IMAGE_LAYOUT_GENERAL) {
            image_memory_barrier.srcAccessMask = 0;
            image_memory_barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

            source_stage = VK_PIPELINE_STAGE_TRANSFER_BIT;
            dst_stages = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
        }

        if (p_old == VK_IMAGE_LAYOUT_UNDEFINED &&
            p_new == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL) {
            image_memory_barrier.srcAccessMask = 0;
            image_memory_barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

            source_stage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
            dst_stages = VK_PIPELINE_STAGE_TRANSFER_BIT;
        } // Convert back from read-only to updateable
        else if (p_old == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL &&
                 p_new == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL) {
            image_memory_barrier.srcAccessMask = VK_ACCESS_SHADER_READ_BIT;
            image_memory_barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

            source_stage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
            dst_stages = VK_PIPELINE_STAGE_TRANSFER_BIT;
        } // Convert from updateable texture to shader read-only
        else if (p_old == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL &&
                 p_new == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL) {
            image_memory_barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
            image_memory_barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

            source_stage = VK_PIPELINE_STAGE_TRANSFER_BIT;
            dst_stages = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
        } // Convert depth texture from undefined state to depth-stencil buffer
        else if (p_old == VK_IMAGE_LAYOUT_UNDEFINED &&
                 p_new == VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL) {
            image_memory_barrier.srcAccessMask = 0;
            image_memory_barrier.dstAccessMask =
              VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT |
              VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;

            source_stage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
            dst_stages = VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
        } // Wait for render pass to complete
        else if (p_old == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL &&
                 p_new == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL) {
            image_memory_barrier.srcAccessMask =
              0; // VK_ACCESS_SHADER_READ_BIT;
            image_memory_barrier.dstAccessMask = 0;
            source_stage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
            dst_stages = VK_PIPELINE_STAGE_ALL_GRAPHICS_BIT;
            dst_stages = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
            source_stage = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
            dst_stages = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
        } // Convert back from read-only to color attachment
        else if (p_old == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL &&
                 p_new == VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL) {
            image_memory_barrier.srcAccessMask = VK_ACCESS_SHADER_READ_BIT;
            image_memory_barrier.dstAccessMask =
              VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

            source_stage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
            dst_stages = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
        } // Convert from updateable texture to shader read-only
        else if (p_old == VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL &&
                 p_new == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL) {
            image_memory_barrier.srcAccessMask =
              VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
            image_memory_barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

            source_stage = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
            dst_stages = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
        } // Convert back from read-only to depth attachment
        else if (p_old == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL &&
                 p_new == VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL) {
            image_memory_barrier.srcAccessMask = VK_ACCESS_SHADER_READ_BIT;
            image_memory_barrier.dstAccessMask =
              VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;

            source_stage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
            dst_stages = VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT;
        } // Convert from updateable depth texture to shader read-only
        else if (p_old == VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL &&
                 p_new == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL) {
            image_memory_barrier.srcAccessMask =
              VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
            image_memory_barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

            source_stage = VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT;
            dst_stages = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
        }

        vkCmdPipelineBarrier(p_command_buffer,
                             source_stage,
                             dst_stages,
                             0,
                             0,
                             nullptr,
                             0,
                             nullptr,
                             1,
                             &image_memory_barrier);
    }


	void queue_submit(const VkQueue& p_queue_handle, const std::span<VkCommandBuffer>& p_commands) {
		VkSubmitInfo submit_info = {
            .sType = VK_STRUCTURE_TYPE_SUBMIT_INFO,
            .commandBufferCount = static_cast<uint32_t>(p_commands.size()),
            .pCommandBuffers = p_commands.data(),
        };

        vkQueueSubmit(p_queue_handle, 1, &submit_info, nullptr);
        vkQueueWaitIdle(p_queue_handle);
	}
};