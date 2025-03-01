#include <drivers/vulkan/vulkan_context.hpp>
#include <drivers/vulkan/vulkan_core.hpp>
#include <vulkan/vulkan_core.h>
#include <drivers/vulkan/helper_functions.hpp>

namespace atlas::vk{
    VkCommandBuffer create_command_buffer_and_record(const VkDevice& p_driver, const VkCommandPool& p_command_buffer){
        VkCommandBufferAllocateInfo cmd_buffer_alloc_info = {
            .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
            .commandPool = p_command_buffer,
            .level = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
            .commandBufferCount = 1
        };

        VkCommandBuffer command_buffer;

        vk_check(vkAllocateCommandBuffers(p_driver, &cmd_buffer_alloc_info, &command_buffer), "vkAllocateCommandBuffers", __FILE__, __LINE__, __FUNCTION__);

        VkCommandBufferBeginInfo cmd_buffer_begin_info = {
            .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
            .flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT
        };

        vk_check(vkBeginCommandBuffer(command_buffer, &cmd_buffer_begin_info), "vkBeginCommandBuffer", __FILE__, __LINE__, __FUNCTION__);

        return command_buffer;
    }


    void begin_command_buffer(const VkCommandBuffer& p_command_buffer){
        VkCommandBufferBeginInfo cmd_buffer_begin_info = {
            .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
            .flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT
        };

        vk_check(vkBeginCommandBuffer(p_command_buffer, &cmd_buffer_begin_info), "vkBeginCommandBuffer", __FILE__, __LINE__, __FUNCTION__);
    }

    void end_command_buffer(const VkCommandBuffer& p_command_bufferContext){
        vkEndCommandBuffer(p_command_bufferContext);
    }

    void submit(VkQueue p_queue_to_use, const std::span<VkCommandBuffer>& p_command_buffers){
        VkSubmitInfo submit_info = {
            .sType = VK_STRUCTURE_TYPE_SUBMIT_INFO,
            .commandBufferCount = static_cast<uint32_t>(p_command_buffers.size()),
            .pCommandBuffers = p_command_buffers.data()
        };

        vkQueueSubmit(p_queue_to_use, static_cast<uint32_t>(p_command_buffers.size()), &submit_info, VK_NULL_HANDLE);
        vkQueueWaitIdle(p_queue_to_use);
    }

    VkImageView create_image_view(VkImage p_image, VkFormat p_surface_format){

        VkImageView new_image_view;
        VkImageViewCreateInfo image_view_create_info = {
            .sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
            .pNext = nullptr,
            .flags = 0,
            .image = p_image,
            .viewType = VK_IMAGE_VIEW_TYPE_2D,
            .format = p_surface_format,
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

        vk_check(vkCreateImageView(vk_context::get_current_driver(), &image_view_create_info, nullptr, &new_image_view), "vkCreateImageView", __FILE__, __LINE__, __FUNCTION__);
        return new_image_view;
    }

    VkImage create_image(VkFormat p_format, uint32_t p_width, uint32_t p_height){
        VkImage new_image;

        VkImageCreateInfo image_ci = {
            .sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO,
            .imageType = VK_IMAGE_TYPE_2D,
            //! @note that vkCmdBlitImage (if supported) will also do format conversions if the swapchain color format would differ
            .format = p_format,
            .extent = {
                .width = p_width,
                .height = p_height,
                .depth = 1
            },
            .mipLevels = 1,
            .arrayLayers = 1,
            .samples = VK_SAMPLE_COUNT_1_BIT,
            .tiling = VK_IMAGE_TILING_LINEAR,
            .usage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
            .initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
        };

        vk_check(vkCreateImage(vk_context::get_current_driver(), &image_ci, nullptr, &new_image), "vkCreateImage", __FILE__, __LINE__, __FUNCTION__);

        return new_image;
    }


    VkCommandBuffer begin_single_time_command_buffer(const VkCommandPool& p_command_pool){
        VkCommandBufferAllocateInfo alloc_info = {
            .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
            .commandPool = p_command_pool,
            .level = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
            .commandBufferCount = 1,
        };

        VkCommandBuffer temp_command_buffer;
        vk_check(vkAllocateCommandBuffers(vk_context::get_current_driver(), &alloc_info, &temp_command_buffer), "vkAllocateCommandBuffers", __FILE__, __LINE__, __FUNCTION__);

        VkCommandBufferBeginInfo command_buffer_begin_info = {
            .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
            .flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT
        };

        vk_check(vkBeginCommandBuffer(temp_command_buffer, &command_buffer_begin_info), "vkBeginCommandBuffer", __FILE__, __LINE__, __FUNCTION__);

        return temp_command_buffer;
    }

    void end_single_time_command_buffer(const VkCommandBuffer& p_command_buffer, const VkCommandPool& p_command_pool){
        vkEndCommandBuffer(p_command_buffer);

        VkSubmitInfo submit_info = {
            .sType = VK_STRUCTURE_TYPE_SUBMIT_INFO,
            .commandBufferCount = 1,
            .pCommandBuffers = &p_command_buffer
        };

        // auto queue_graphics_idx = vk_context::get_current_selected_physical_driver().get_queue_indices().Graphics;
        auto graphics_queue = vk_context::get_current_driver().get_graphics_queue();

        vk_check(vkQueueSubmit(graphics_queue, 1, &submit_info, VK_NULL_HANDLE), "vkQueueSubmit", __FILE__, __LINE__, __FUNCTION__);
        vk_check(vkQueueWaitIdle(graphics_queue), "vkQueueWaitIdle", __FILE__, __LINE__, __FUNCTION__);

        vkFreeCommandBuffers(vk_context::get_current_driver(), p_command_pool, 1, &p_command_buffer);
    }

    VkCommandPool create_command_pool(){
        VkCommandPoolCreateInfo cmd_pool_ci = {
            .sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
            .flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT,
            .queueFamilyIndex = vk_context::get_current_selected_physical_driver().get_queue_indices().Graphics
        };

        VkCommandPool temp_command_pool;

        vk_check(vkCreateCommandPool(vk_context::get_current_driver(), &cmd_pool_ci, nullptr, &temp_command_pool), "vkCreateCommandPool", __FILE__, __LINE__, __FUNCTION__);
        
        return temp_command_pool;
    }

    void pipeline_image_barrier(VkCommandBuffer p_command_buffer, VkImage p_image, VkAccessFlags p_src_access_mask, VkAccessFlags p_dst_access_mask, VkImageLayout p_old_image_layout, VkImageLayout p_new_image_layout, VkPipelineStageFlags p_src_stage_flags, VkPipelineStageFlags p_dst_stage_flags, VkImageSubresourceRange p_subresource_range){
        VkImageMemoryBarrier image_memory_barrier = {
            .sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
            .srcAccessMask = p_src_access_mask,
            .dstAccessMask = p_dst_access_mask,
            .oldLayout = p_old_image_layout,
            .newLayout = p_new_image_layout,
            .srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
            .dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
            .image = p_image,
            .subresourceRange = p_subresource_range
        };

        vkCmdPipelineBarrier(p_command_buffer, p_src_stage_flags, p_dst_stage_flags, 0, 0, nullptr, 0, nullptr, 1, &image_memory_barrier);
    }
};