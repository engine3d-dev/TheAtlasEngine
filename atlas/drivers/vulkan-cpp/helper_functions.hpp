#pragma once
#include <string>
#include <vulkan/vulkan_core.h>
#include <drivers/vulkan-cpp/vk_types.hpp>
#include <source_location>

namespace atlas::vk {

    /**
     * @param p_result checks if the result of a vulkan handler was created
     * correctly
     * @param p_name used for debugging of which handler failed
     * @param p_source is the location of the call-site that invoked vk_check
     */
    void vk_check(
      const VkResult& p_result,
      const std::string& p_name,
      const std::source_location& p_source = std::source_location::current());

    /**
     * @brief Validate if the format specified is a stencil attachment
     */
    bool has_stencil_attachment(VkFormat p_format);

    //! @return stringified version of the `VkFormat`
    std::string vk_format_to_string(VkFormat p_format);

    //! @return the image size based on the capabilities of the vulkan
    //! `VkSurfaceKHR` that is supported
    uint32_t select_images_size(
      const VkSurfaceCapabilitiesKHR& p_surface_capabilities);

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
    // void transition_image_layout(VkImage& p_image,
    //                              VkFormat p_format,
    //                              VkImageLayout p_old,
    //                              VkImageLayout p_new);

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
    void transition_image_layout(VkCommandBuffer& p_command_buffer,
                                 VkImage& p_image,
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