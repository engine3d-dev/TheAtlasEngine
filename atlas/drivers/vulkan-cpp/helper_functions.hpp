#pragma once
#include <string>
#include <span>
#include <vulkan/vulkan_core.h>
#include <drivers/vulkan-cpp/vk_types.hpp>
#include <drivers/vulkan-cpp/vk_command_buffer.hpp>

namespace atlas::vk {
    // const char* vk_to_string(VkResult res);
    //! @note Terminates if the result was unsuccessful!
    //! @note TODO --- We shouldn't std::terminate, look into alternatives.
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
	
	void copy(vk_command_buffer& p_command_buffer, VkImage& p_image,
                                  VkBuffer& p_buffer,
                                  uint32_t p_width,
                                  uint32_t p_height);
	
	

    VkCommandPool create_single_command_pool();

    VkCommandBuffer create_single_command_buffer(
      const VkCommandPool& p_command_pool);

    // VkCommandBufferBeginInfo command_buffer_begin_info(
    //   const VkCommandBufferUsageFlags& p_usage);

    void begin_command_buffer(const VkCommandBuffer& p_command_buffer,
                              VkCommandBufferUsageFlags p_usage_flags);

    void end_command_buffer(const VkCommandBuffer& p_command_buffer);

    VkSemaphore create_semaphore(const VkDevice& p_driver);

    vk_buffer create_buffer(const vk_buffer_info& p_info);

	//! @brief Copies from one buffer source into another buffer source with a specific size of bytes to be stored the buffer that is being copied to
	void copy(const vk_buffer& p_src, const vk_buffer& p_dst, size_t p_size_of_bytes);

    // // Use is for vkMap/vkUnmap data of bytes yourself
    void write(const vk_buffer& p_buffer,
               const void* p_data,
               size_t p_size_in_bytes);

    // // Maps/Unmaps a buffer handler from std::span<uint32_t>
    void write(const vk_buffer& p_buffer,
               const std::span<uint32_t>& p_in_buffer);
	
	// // Maps/Unmaps a buffer handler from std::span<float>
    // void write(const vk_buffer& p_buffer,
    //            const std::span<float>& p_in_buffer);
	
	// // Maps/Unmaps a buffer handler from std::span<vertex>
    void write(const vk_buffer& p_buffer,
               const std::span<vertex>& p_in_buffer);
    
	// // Copies from one buffer source into another buffer source with a specific size of bytes to be stored the buffer that is being copied to
	// void copy(const vk_buffer& p_src, const vk_buffer& p_dst, uint32_t p_size_of_bytes);

	/**
	 * @param VkFormat input is the format to check if this format's a stencil attachment
	*/
	bool has_stencil_attachment(VkFormat p_format);
	
	/**
	 * @name image_memory_barrier
	 * @brief pipeline barriers are generally used to synchronize access to resources
	 * @brief Ensuring that writing to a buffer completes before reading from it, but also used when transitioning image layouts
	 * 		  and transfer queue's family ownership when VK_SHARING_MODE_EXCLUSIVE is used.
	 * @param VkCommandBuffer is the command buffer we are executing after recording this operation with
	 * @param VkImage is the image that will use our image we are transitioning
	 * @param VkFormat specifies the format of pixel data has been loaded
	 * @param p_old is the image layout currently defined with the VkImage
	 * @param p_new is the image layout that we are going to be modifying the VkImage into
	 * @brief This function creates a copy command buffer
	 * @brief Meaning it should accept a created command buffer from the user rather than constructing one itself
	*/
	// void image_memory_barrier(VkCommandBuffer& p_command_buffer,
	// 	VkImage& p_image,
	// 	VkFormat p_format,
	// 	VkImageLayout p_old,
	// 	VkImageLayout p_new);
	
	/**
	 * @name transition_image_layout
	 * @param VkImage is the image we want to transition to
	 * @param VkFormat specifying the format in which are image has been set to
	 * @param p_old is a VkImageLayout that is the old layout image was previously
	 * @param p_new is VkImageLayout that is the new layout the image will be transitioning to
	 * 
	 * @brief Records and executes vkCmdCopyBufferToImage vulkan function
	 * @brief Used for transitioning between a given set of image layouts into a completely different layout
	 * @brief Helpful for ensuring the pixel data the image contains
	 * @brief This function creates a copy command buffer
	 * @brief Meaning it should accept a created command buffer from the user rather than constructing one itself
	*/
    void transition_image_layout(VkImage& p_image,
		VkFormat p_format,
		VkImageLayout p_old,
		VkImageLayout p_new);

    // const char* vk_to_string(VkResult res);
    //! @note Terminates if the result was unsuccessful!
    //! @note TODO --- I dont think this should std::terminate, but it should at least throw an issue.
	//! @note Something to do is have specific functions for handling in specifics of critical errors happening
    // void vk_check(const VkResult& p_result,
    //               const char* p_tag,
    //               const char* p_function_name,
    //               const char* p_filepath = __FILE__,
    //               uint32_t p_line = __LINE__);

    // void vk_check_format(VkFormat p_format,
    //                      const char* p_function_name,
    //                      const char* p_filepath = __FILE__,
    //                      uint32_t p_line = __LINE__);

    std::string vk_queue_flags_to_string(VkQueueFlagBits p_flags);

    std::string vk_present_mode_to_string(VkPresentModeKHR p_present_mode);

    std::string vk_format_to_string(VkFormat p_format);
};