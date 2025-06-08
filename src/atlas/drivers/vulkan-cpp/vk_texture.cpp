#include <drivers/vulkan-cpp/vk_texture.hpp>
#include <core/image/stb_image.hpp>
#include <core/engine_logger.hpp>
#include <drivers/vulkan-cpp/vk_context.hpp>
#include <drivers/vulkan-cpp/helper_functions.hpp>

namespace atlas::vk {

    int bytes_per_texture_format(VkFormat p_format) {
        switch (p_format) {
            case VK_FORMAT_R8_SINT:
            case VK_FORMAT_R8_UNORM:
                return 1;
            case VK_FORMAT_R16_SFLOAT:
                return 2;
            case VK_FORMAT_R16G16_SFLOAT:
            case VK_FORMAT_B8G8R8A8_UNORM:
            case VK_FORMAT_R8G8B8A8_UNORM:
                return 4;
            case VK_FORMAT_R16G16B16A16_SFLOAT:
                return 4 * sizeof(uint16_t);
            case VK_FORMAT_R32G32B32A32_SFLOAT:
                return 4 * sizeof(float);
            default:
                console_log_fatal("Error unknown format!!!");
                return 0;
        }

        return 0;
    }

    texture::texture([[maybe_unused]] const std::filesystem::path& p_filepath) {
        m_driver = vk_context::driver_context();

        command_buffer_settings settings = {
            0,
            command_buffer_levels::Primary,
            VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT,
        };

        m_copy_command_buffer = vk_command_buffer(settings);

        // 1. load from file
        int w, h;
        int channels;
        stbi_uc* image_pixel_data = stbi_load(p_filepath.string().c_str(), &w, &h, &channels, STBI_rgb_alpha);

        m_width = w;
        m_height = h;

        if(!image_pixel_data) {
            console_log_error("Could not load filepath {}", p_filepath.string());
            m_is_image_loaded = false;
            return;
        }
        else {
            console_log_info("Loading filepath = {} successful", p_filepath.string());
        }

        // 2. Create Image Data
        // 2.1 Updating Texture data
        VkFormat format = VK_FORMAT_R8G8B8A8_UNORM;
        create_texture_from_data(w, h, image_pixel_data, format);
        stbi_image_free(image_pixel_data);

        // 3.) Create Image View
        VkImageAspectFlags aspect_flags = VK_IMAGE_ASPECT_COLOR_BIT;
        m_texture_image.ImageView = create_image_view(m_texture_image.Image, format, aspect_flags);

        vk_filter_range sampler_range = {
            .min = VK_FILTER_LINEAR,
            .max = VK_FILTER_LINEAR
        };

        VkSamplerAddressMode addr_mode = VK_SAMPLER_ADDRESS_MODE_REPEAT;

        m_texture_image.Sampler = create_sampler(sampler_range, addr_mode);
        m_is_image_loaded = true;
    }

    void texture::create_texture_from_data(uint32_t p_width, uint32_t p_height, const void* p_data, const VkFormat& p_format) {
        VkImageUsageFlagBits usage = (VkImageUsageFlagBits)(VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT);
        VkMemoryPropertyFlagBits property = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;

        // 1. create image  object
        m_texture_image = create_image2d(p_width, p_height, p_format, usage, property);

        // 2. update texture data
        update_texture(m_texture_image, p_width, p_height, p_format, p_data);

        console_log_fatal("create_texture_from_data update END initialization!!!\n\n");
    }

    void texture::update_texture(vk_image& p_image, uint32_t p_width, uint32_t p_height, VkFormat p_format, const  void* p_data) {
        // 1. bytes per pixels
        int bytes_per_pixels = bytes_per_texture_format(p_format);

        // 2. layer_size
        uint32_t layer_size = p_width * p_height * bytes_per_pixels;
        int layer_count = 1;
        uint32_t image_size = layer_count * layer_size;

        VkBufferUsageFlags usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
        VkMemoryPropertyFlags property = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
                                         VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;

        // 3. creating data for staging buffers
        // m_staging_buffer = create_buffer(image_size, usage, property);

        vk_buffer_info staging_info = {
            .device_size = (uint32_t)image_size,
            .usage = usage,
            .memory_property_flag = property
        };
        m_staging_buffer = create_buffer(staging_info);

        // 4. maps the buffer data to that parameters
        write(m_staging_buffer, p_data, image_size);

        // 5. transition image layout
        transition_image_layout(p_image.Image,
                                p_format,
                                VK_IMAGE_LAYOUT_UNDEFINED,
                                VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);

        // // 6. Copy buffer to image
        copy(m_copy_command_buffer, p_image.Image,
                             m_staging_buffer.handler,
                             p_width,
                             p_height);

        // // 7. transition image layout again
        transition_image_layout(p_image.Image,
                                p_format,
                                VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                                VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

        console_log_error("create_texture_from_data end initialization successfully!!!");
    }

    void texture::destroy() {
        vkDestroyImageView(m_driver, m_texture_image.ImageView, nullptr);
        vkDestroyImage(m_driver, m_texture_image.Image, nullptr);
        vkDestroySampler(m_driver, m_texture_image.Sampler, nullptr);

        vkFreeMemory(m_driver, m_texture_image.DeviceMemory, nullptr);

        vkDestroyBuffer(m_driver, m_staging_buffer.handler, nullptr);
        vkFreeMemory(m_driver, m_staging_buffer.device_memory, nullptr);

        m_copy_command_buffer.destroy();
    }
};