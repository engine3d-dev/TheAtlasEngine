#include <drivers/vulkan-cpp/vk_texture.hpp>
#include <core/image/stb_image.hpp>
#include <core/engine_logger.hpp>
#include <drivers/vulkan-cpp/vk_context.hpp>
#include <drivers/vulkan-cpp/helper_functions.hpp>
#include <drivers/vulkan-cpp/utilties/utils.hpp>
#include <array>

namespace atlas::vk {

    static vk_image create_texture_from_data(
      const VkDevice& p_driver,
      const texture_properties& p_properties,
      const void* p_data) {

        command_buffer_settings settings = {
            0,
            command_buffer_levels::primary,
            VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT,
        };

        vk_command_buffer copy_command_buffer = vk_command_buffer(settings);

        // 1. create image object
        vk_image texture_image = create_image2d(p_properties.width,
                                                p_properties.height,
                                                p_properties.format,
                                                p_properties.usage,
                                                p_properties.property);

        // 2. update texture data
        // bytes per pixels
        int bytes_per_pixels = bytes_per_texture_format(p_properties.format);

        // 2.2 layer_size
        uint32_t layer_size =
          p_properties.width * p_properties.height * bytes_per_pixels;
        int layer_count = 1;
        uint32_t image_size = layer_count * layer_size;

        // 2.3 transfer data from staging buffer
        vk_buffer_info staging_info = {
            .device_size = (uint32_t)image_size,
            .usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
            .memory_property_flag = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
                                    VK_MEMORY_PROPERTY_HOST_CACHED_BIT,
        };

        vk_buffer staging_buffer = create_buffer(staging_info);

        // 4. maps the buffer data to that parameters
        write(staging_buffer, p_data, image_size);

        // 5. transition image layout
        // copy_command_buffer.begin(VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT);
        transition_image_layout(texture_image.image,
                                p_properties.format,
                                VK_IMAGE_LAYOUT_UNDEFINED,
                                VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);

        // // 6. Copy buffer to image
        copy(copy_command_buffer,
             texture_image.image,
             staging_buffer.handler,
             p_properties.width,
             p_properties.height);

        // // 7. transition image layout again
        transition_image_layout(texture_image.image,
                                p_properties.format,
                                VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                                VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

        copy_command_buffer.destroy();

        free_buffer(p_driver, staging_buffer);

        return texture_image;
    }

    texture::texture(const texture_extent& p_extent) {
        m_driver = vk_context::driver_context();

        command_buffer_settings settings = {
            0,
            command_buffer_levels::primary,
            VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT,
        };

        // 1.) Load in extent dimensions
		// Loading in raw white pixels for our texture.
		// TODO: Take in a std::span<uint8_t> for pixels that will then be written to the texture
		std::array<uint8_t, 4> white_color = {0xFF, 0xFF, 0xFF, 0xFF};

        m_width = p_extent.width;
        m_height = p_extent.height;

        texture_properties properties = {
            .width = m_width,
            .height = m_height,
            .usage = (VkImageUsageFlagBits)(VK_IMAGE_USAGE_TRANSFER_DST_BIT |
                                            VK_IMAGE_USAGE_SAMPLED_BIT),
            .property = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
            // .format = VK_FORMAT_R8G8B8A8_UNORM,
			.format = VK_FORMAT_R8G8B8A8_SRGB
			// .format = VK_FORMAT_R64G64B64A64_SFLOAT
        };
        m_texture_image = create_texture_from_data(
          m_driver, properties, white_color.data());

        // 3.) Create Image View
        VkImageAspectFlags aspect_flags = VK_IMAGE_ASPECT_COLOR_BIT;
        m_texture_image.image_view = create_image_view(
          m_texture_image.image, properties.format, aspect_flags);

        vk_filter_range sampler_range = { .min = VK_FILTER_LINEAR,
                                          .max = VK_FILTER_LINEAR };

        VkSamplerAddressMode addr_mode = VK_SAMPLER_ADDRESS_MODE_REPEAT;

        m_texture_image.sampler = create_sampler(sampler_range, addr_mode);
        m_is_image_loaded = true;
    }

    texture::texture(const std::filesystem::path& p_filepath) {
        m_driver = vk_context::driver_context();
        command_buffer_settings settings = {
            0,
            command_buffer_levels::primary,
            VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT,
        };

        // 1. load from file
        int w, h;
        int channels;
        stbi_uc* image_pixel_data = stbi_load(
          p_filepath.string().c_str(), &w, &h, &channels, STBI_rgb_alpha);

        m_width = w;
        m_height = h;

        if (!image_pixel_data) {
            m_is_image_loaded = false;
            return;
        }

        texture_properties texture_settings = {
            .width = m_width,
            .height = m_height,
            .usage = (VkImageUsageFlagBits)(VK_IMAGE_USAGE_TRANSFER_DST_BIT |
                                            VK_IMAGE_USAGE_SAMPLED_BIT),
            .property = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
            .format = VK_FORMAT_R8G8B8A8_UNORM,
        };
        m_texture_image = create_texture_from_data(
          m_driver, texture_settings, image_pixel_data);

        stbi_image_free(image_pixel_data);

        // 3.) Create Image View
        VkImageAspectFlags aspect_flags = VK_IMAGE_ASPECT_COLOR_BIT;
        m_texture_image.image_view = create_image_view(
          m_texture_image.image, texture_settings.format, aspect_flags);

        vk_filter_range sampler_range = { .min = VK_FILTER_LINEAR,
                                          .max = VK_FILTER_LINEAR };

        VkSamplerAddressMode addr_mode = VK_SAMPLER_ADDRESS_MODE_REPEAT;

        m_texture_image.sampler = create_sampler(sampler_range, addr_mode);
        m_is_image_loaded = true;
    }

    void texture::destroy() {
        free_image(m_driver, m_texture_image);
    }
};