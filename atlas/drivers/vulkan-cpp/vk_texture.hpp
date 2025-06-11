#pragma once
#include <filesystem>
#include <vulkan/vulkan.h>
#include <drivers/vulkan-cpp/vk_types.hpp>
#include <drivers/vulkan-cpp/vk_command_buffer.hpp>

namespace atlas::vk {
    class texture {
    public:
        texture() = default;
        texture(const std::filesystem::path& p_filepath);


        [[nodiscard]] bool loaded() const { return m_is_image_loaded; }

        [[nodiscard]] vk_image data() const { return m_texture_image; }

        [[nodiscard]] VkImageView image_view() const { return m_texture_image.image_view; }

        [[nodiscard]] VkImage image() const { return m_texture_image.image; }

        [[nodiscard]] VkSampler sampler() const { return m_texture_image.sampler; }

        void update_texture(vk_image& p_image, uint32_t p_width, uint32_t p_height, VkFormat p_format, const  void* p_data);

        void destroy();

        [[nodiscard]] uint32_t width() const;
        
        [[nodiscard]] uint32_t height() const;

    private:
        void create_texture_from_data(uint32_t p_width, uint32_t p_height, const void* p_data, const VkFormat& p_format);

    private:
        VkDevice m_driver=nullptr;
        bool m_is_image_loaded=false;
        vk_buffer m_staging_buffer{};
        vk_image m_texture_image{};
        vk_command_buffer m_copy_command_buffer{};
        uint32_t m_width=0;
        uint32_t m_height=0;
    };
};