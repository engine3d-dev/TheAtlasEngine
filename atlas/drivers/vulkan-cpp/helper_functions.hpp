#pragma once
#include <string>
#include <span>
#include <vulkan/vulkan_core.h>
#include <drivers/vulkan-cpp/vk_types.hpp>
#include <drivers/vulkan-cpp/vk_command_buffer.hpp>
#include <vector>

namespace atlas::vk {

    void vk_check(VkResult p_result,
                  const char* p_tag,
                  const char* p_filepath,
                  uint32_t p_line,
                  const char* p_function_name);

    void vk_check_format(VkFormat p_format,
                         const char* p_filepath,
                         uint32_t p_line,
                         const char* p_function_name);

    std::string vk_queue_flags_to_string(VkQueueFlagBits p_flags);

    VkImageView create_image_view(const VkImage& p_image,
                                  VkFormat p_format,
                                  VkImageAspectFlags p_aspect_flags);

    VkImageView create_image_view(const VkDevice& p_driver,
                                  VkImage p_image,
                                  VkSurfaceFormatKHR p_surface_format,
                                  VkImageAspectFlags p_aspect_flags,
                                  VkImageViewType p_view_t,
                                  uint32_t p_layer_count,
                                  uint32_t p_mip_levels);

    vk_image create_image2d(uint32_t p_width,
                            uint32_t p_height,
                            VkFormat p_format,
                            VkImageUsageFlags p_usage,
                            VkMemoryPropertyFlagBits p_property);

    VkSampler create_sampler(const vk_filter_range& p_range,
                             VkSamplerAddressMode p_address_mode);

    // void copy(vk_command_buffer& p_command_buffer,
    //           VkImage& p_image,
    //           VkBuffer& p_buffer,
    //           uint32_t p_width,
    //           uint32_t p_height);

    // VkCommandPool create_single_command_pool();

    // VkCommandBuffer create_single_command_buffer(
    //   const VkCommandPool& p_command_pool);

    // void begin_command_buffer(const VkCommandBuffer& p_command_buffer,
    //                           VkCommandBufferUsageFlags p_usage_flags);

    // void end_command_buffer(const VkCommandBuffer& p_command_buffer);

	//! @brief Converts from command_buffer_levels struct to vulkan's direct types of VkCommandBufferLevel
	VkCommandBufferLevel to_vk_command_buffer_level(const command_buffer_levels& p_levels);

	//! @return new semaphore handle
    VkSemaphore create_semaphore(const VkDevice& p_driver);
	
	//! @return new vk_buffer handler
    vk_buffer create_buffer(const vk_buffer_info& p_info);

    //! @brief Copies from one buffer source into another buffer source with a
    //! specific size of bytes to be stored the buffer that is being copied to
    void copy(const vk_buffer& p_src,
              const vk_buffer& p_dst,
              size_t p_size_of_bytes);

    /**
	 * @brief maps the current buffer handler to some block of memory and the byte size for that chunk
	*/
    /*
    void write(const vk_buffer& p_buffer,
               const void* p_data,
               size_t p_size_in_bytes);
    */

	/**
	 * @brief Maps buffer handler to chunk of data of type, that is std::span<uint32_t>.
	*/
    /*
    void write(const vk_buffer& p_buffer,
               const std::span<uint32_t>& p_in_buffer);
    */

	/**
	 * @brief Maps buffer handler to data chunks that contain vertices
	*/
    /*
    void write(const vk_buffer& p_buffer,
               const std::span<vertex_input>& p_in_buffer);
    */

    /**
	 * @brief Validate if the format specified is a stencil attachment
     */
    bool has_stencil_attachment(VkFormat p_format);

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
	// void image_memory_barrier(VkCommandBuffer& p_command_buffer,
    //                           VkImage& p_image,
    //                           VkFormat p_format,
    //                           VkImageLayout p_old,
    //                           VkImageLayout p_new);


    // /**
    //  * @name transition_image_layout
    //  * @param VkImage is the image we want to transition to
    //  * @param VkFormat specifying the format in which are image has been set to
    //  * @param p_old is a VkImageLayout that is the old layout image was
    //  * previously
    //  * @param p_new is VkImageLayout that is the new layout the image will be
    //  * transitioning to
    //  *
    //  * @brief Records and executes vkCmdCopyBufferToImage vulkan function
    //  * @brief Used for transitioning between a given set of image layouts into a
    //  * completely different layout
    //  * @brief Helpful for ensuring the pixel data the image contains
    //  * @brief This function creates a copy command buffer
    //  * @brief Meaning it should accept a created command buffer from the user
    //  * rather than constructing one itself
	// */
    // void transition_image_layout(VkImage& p_image,
    //                              VkFormat p_format,
    //                              VkImageLayout p_old,
    //                              VkImageLayout p_new);
	
	//! @return stringified version of the `VkQueueFlagBits` flags
    std::string vk_queue_flags_to_string(VkQueueFlagBits p_flags);

	//! @return stringified version of the `VkPresentModeKHR`
    std::string vk_present_mode_to_string(VkPresentModeKHR p_present_mode);

	//! @return stringified version of the `VkFormat`
    std::string vk_format_to_string(VkFormat p_format);


    VkShaderStageFlags to_vk_shader_stage(const shader_stage& p_stage);
	
	VkShaderStageFlagBits to_vk_shader_stage_bits(const shader_stage& p_stage);

	//! @return vulkan buffer handler that is initialized as a uniform buffer handler
	vk_buffer create_uniform_buffer(uint32_t p_size);
	
	//! @return Returns the VkDescriptorType that represents a "handle" the shader resource is acecssing
	VkDescriptorType to_vk_descriptor_type(const buffer& p_type);

	//! @return the bytes of the texture format specified by the parameter "VkFormat"
    int bytes_per_texture_format(VkFormat p_format);

	//! @return `VkPresentModeKHR` by selecting the current compatiable presentation modes available
    VkPresentModeKHR select_compatible_present_mode(
      const VkPresentModeKHR& p_request,
      const std::span<VkPresentModeKHR>& p_modes);
    
	//! @return the image size based on the capabilities of the vulkan `VkSurfaceKHR` that is supported
    uint32_t select_images_size(
      const VkSurfaceCapabilitiesKHR& p_surface_capabilities);
};