module;

#include <string_view>
#include <vector>
#include <span>
#include <cstdint>
#include <vulkan/vulkan.h>

#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#endif

export module atlas.drivers.vulkan:stb_image;

import vk;

export namespace atlas {

    /**
     * @brief Implementing image loading that utilizes stb_image to decode
     * disk-based images into uncompressed streams of bytes.
     *
     * Provides size extents of uncompressed of the total image size.
     *
     */
    class stb_image : public vk::image {
    public:
        stb_image() = default;

        stb_image(std::string_view p_path, const vk::texture_params& p_params) {
            image_load(p_path, p_params);
        }

        ~stb_image() = default;

    protected:
        bool image_load(std::string_view p_path,
                        vk::texture_params p_params) override {
            int w = 0;
            int h = 0;
            int channels = 0;

            stbi_uc* image_pixel_data =
              stbi_load(p_path.data(), &w, &h, &channels, STBI_rgb_alpha);

            if (!image_pixel_data) {
                return false;
            }

            const VkFormat texture_format =
              static_cast<VkFormat>(vk::format::r8g8b8a8_unorm);
            int bytes_per_pixel = vk::bytes_per_texture_format(texture_format);

            m_extent = {
                .width = static_cast<uint32_t>(w),
                .height = static_cast<uint32_t>(h),
            };

            // Retrieving total size of bytes of the dimensions of the image and
            // accounting for pixels of the image
            uint32_t size_bytes =
              m_extent.width * m_extent.height * bytes_per_pixel;

            // Retrieving total image size to the count of the image layers
            uint32_t size = size_bytes * p_params.layer_count;

            m_bytes.reserve(size);
            std::span<uint8_t> bytes_view =
              std::span<uint8_t>(image_pixel_data, size);

            m_bytes.assign(bytes_view.begin(), bytes_view.end());

            stbi_image_free(image_pixel_data);

            return true;
        }

        [[nodiscard]] std::span<const uint8_t> image_read() const override {
            return m_bytes;
        }

        [[nodiscard]] vk::image_extent image_extent() const override {
            return m_extent;
        }

    private:
        vk::image_extent m_extent{};
        std::vector<uint8_t> m_bytes{};
    };
};