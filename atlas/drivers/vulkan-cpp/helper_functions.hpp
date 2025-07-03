#pragma once
#include <string>
#include <span>
#include <vulkan/vulkan_core.h>
#include <drivers/vulkan-cpp/vk_types.hpp>
#include <drivers/vulkan-cpp/vk_command_buffer.hpp>

namespace atlas::vk {

    //! @brief Checks for valid `VkResult` was successful
    void vk_check(VkResult p_result,
                  const char* p_tag,
                  const char* p_filepath,
                  uint32_t p_line,
                  const char* p_function_name);

    //! @brief Checks for valid `VkFormat` specified.
    void vk_check_format(VkFormat p_format,
                         const char* p_filepath,
                         uint32_t p_line,
                         const char* p_function_name);

    /**
     * @param p_image is the image handler to create a view handler from
     * @param p_format specify format to create the VkImageView handler from
     * @param p_aspect_flag to specify the data this VkImageView handler will
     * contain
     */
    VkImageView create_image_view(const VkImage& p_image,
                                  VkFormat p_format,
                                  VkImageAspectFlags p_aspect_flags);

    /**
     * @return new VkImageView handler object
     * @param p_driver the driver to create the new handler
     * @param p_surface_format specify a surface format to create the
     * VkImageView handler with
     * @param p_aspect_flag to specify what kind of aspect of data this
     * VkImageView will contain
     * @param p_mip_level default to 1; used to set mip levels if required
     */
    VkImageView create_image_view(const VkDevice& p_driver,
                                  VkImage p_image,
                                  VkSurfaceFormatKHR p_surface_format,
                                  VkImageAspectFlags p_aspect_flags,
                                  VkImageViewType p_view_t,
                                  uint32_t p_layer_count,
                                  uint32_t p_mip_level = 1);

    //! @brief Returns vk_image which contains the VkImage and VkImage handlers
    //! with image 2D specifications
    vk_image create_image2d(uint32_t p_width,
                            uint32_t p_height,
                            VkFormat p_format,
                            VkImageUsageFlags p_usage,
                            VkMemoryPropertyFlagBits p_property);

    VkSampler create_sampler(const vk_filter_range& p_range,
                             VkSamplerAddressMode p_address_mode);

    VkShaderStageFlags to_vk_shader_stage(const shader_stage& p_stage);

    VkShaderStageFlagBits to_vk_shader_stage_bits(const shader_stage& p_stage);

    //! @return vulkan buffer handler that is initialized as a uniform buffer
    //! handler
    vk_buffer create_uniform_buffer(uint32_t p_size);

    //! @return Returns the VkDescriptorType that represents a "handle" the
    //! shader resource is acecssing
    VkDescriptorType to_vk_descriptor_type(const buffer& p_type);

    //! @brief Conversion function that returns VkCommandBufferLevel
    VkCommandBufferLevel to_vk_command_buffer_level(
      const command_buffer_levels& p_levels);

    //! @return new semaphore handle
    VkSemaphore create_semaphore(const VkDevice& p_driver);

    //! @return new vk_buffer handler
    vk_buffer create_buffer(const vk_buffer_info& p_info);

    /**
     * @brief Validate if the format specified is a stencil attachment
     */
    bool has_stencil_attachment(VkFormat p_format);

    //! @return stringified version of the `VkPresentModeKHR`
    std::string vk_present_mode_to_string(VkPresentModeKHR p_present_mode);

    //! @return stringified version of the `VkFormat`
    std::string vk_format_to_string(VkFormat p_format);

    //! @return the bytes of the texture format specified by the parameter
    //! "VkFormat"
    int bytes_per_texture_format(VkFormat p_format);

    //! @return `VkPresentModeKHR` by selecting the current compatiable
    //! presentation modes available
    VkPresentModeKHR select_compatible_present_mode(
      const VkPresentModeKHR& p_request,
      const std::span<VkPresentModeKHR>& p_modes);

    //! @return the image size based on the capabilities of the vulkan
    //! `VkSurfaceKHR` that is supported
    uint32_t select_images_size(
      const VkSurfaceCapabilitiesKHR& p_surface_capabilities);
};