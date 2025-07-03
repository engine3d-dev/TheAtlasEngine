#pragma once
#include <span>
#include <vulkan/vulkan_core.h>
#include <drivers/vulkan-cpp/vk_types.hpp>
#include <drivers/vulkan-cpp/vk_command_buffer.hpp>

namespace atlas::vk {

    void begin_command_buffer(const VkCommandBuffer& p_command_buffer,
                              VkCommandBufferUsageFlags p_usage_flags);

    void end_command_buffer(const VkCommandBuffer& p_command_buffer);

    VkCommandPool create_single_command_pool();

    VkCommandBuffer create_single_command_buffer(
      const VkCommandPool& p_command_pool);

    void copy(vk_command_buffer& p_command_buffer,
              VkImage& p_image,
              VkBuffer& p_buffer,
              uint32_t p_width,
              uint32_t p_height);

    //! @brief Copies from one buffer source into another buffer source with a
    //! specific size of bytes to be stored the buffer that is being copied to
    void copy(const vk_buffer& p_src,
              const vk_buffer& p_dst,
              size_t p_size_of_bytes);

    /**
     * @brief maps the current buffer handler to some block of memory and the
     * byte size for that chunk
     */
    void write(const vk_buffer& p_buffer,
               const void* p_data,
               size_t p_size_in_bytes);

    /**
     * @brief Maps buffer handler to chunk of data of type, that is
     * std::span<uint32_t>.
     */
    void write(const vk_buffer& p_buffer,
               const std::span<uint32_t>& p_in_buffer);

    /**
     * @brief Maps buffer handler to data chunks that contain vertices
     */
    void write(const vk_buffer& p_buffer,
               const std::span<vertex_input>& p_in_buffer);

    /**
     * @name transition_image_layout
     * @param VkImage is the image we want to transition to
     * @param VkFormat specifying the format in which are image has been set to
     * @param p_old is a VkImageLayout that is the old layout image was
     * previously
     * @param p_new is VkImageLayout that is the new layout the image will be
     * transitioning to
     *
     * @brief Records and executes vkCmdCopyBufferToImage vulkan function
     * @brief Used for transitioning between a given set of image layouts into a
     * completely different layout
     * @brief Helpful for ensuring the pixel data the image contains
     * @brief This function creates a copy command buffer
     * @brief Meaning it should accept a created command buffer from the user
     * rather than constructing one itself
     */
    void transition_image_layout(VkImage& p_image,
                                 VkFormat p_format,
                                 VkImageLayout p_old,
                                 VkImageLayout p_new);

    /**
     * @name image_memory_barrier
     * @brief pipeline barriers are generally used to synchronize access to
     * resources.
     *
     * Ensuring that writing to a buffer completes before reading from
     * it, but also used when transitioning image layouts and transfer queue's
     * family ownership when VK_SHARING_MODE_EXCLUSIVE is used.
     *
     * @param VkCommandBuffer is the command buffer we are executing after
     * recording this operation
     * @param VkImage is the image that will use our image we are transitioning
     * @param VkFormat specifies the format of pixel data has been loaded
     * @param p_old is the image layout currently defined with the VkImage
     * @param p_new is the image layout that we are going to be modifying the
     * VkImage into
     * @brief This function creates a copy command buffer
     * @brief Meaning it should accept a created command buffer from the user
     * rather than constructing one itself
     */
    void image_memory_barrier(VkCommandBuffer& p_command_buffer,
                              VkImage& p_image,
                              VkFormat p_format,
                              VkImageLayout p_old,
                              VkImageLayout p_new);

};