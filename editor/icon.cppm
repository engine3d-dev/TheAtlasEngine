module;

#include <flecs.h>
#include <imgui.h>
#include <imgui/backends/imgui_impl_glfw.h>
#include <imgui/backends/imgui_impl_vulkan.h>
#include <filesystem>
#include <span>

export module editor:icon;

import atlas.core.utilities;
import atlas.drivers.vulkan.physical_device;
import atlas.drivers.vulkan.stb_image;
import vk;

export namespace ui::experimental {
    /**
     * @brief This is an experimental feature for setting up an image
     * thumbnail-like abstraction
     * TODO: This should be considered to being abstracted in another approach
     * as this is a temporary solution for it. OR this could be the way we
     * handle icons for the time being for simplicity.
     */
    class icon {
    public:
        icon() = default;
        icon(const VkDevice& p_device,
             uint32_t p_memory_properties,
             const std::filesystem::path& p_filename) {
            // vk::texture_info config_texture = {
            //     .phsyical_memory_properties = p_memory_properties,
            //     .filepath = p_filename
            // };
            vk::texture_params config_texture = {
                // .memory_mask = physical_device.memory_properties(
                // vk::memory_property::host_visible_bit |
                // vk::memory_property::host_cached_bit),
                .memory_mask = p_memory_properties,
            };

            atlas::vulkan::stb_image image(p_filename.string(), config_texture);
            m_icon_image = vk::texture(p_device, &image, config_texture);
            if (!m_icon_image.loaded()) {
                console_log_info("Play Button Could not be loaded!!");
            }

            m_extent = image.extent();
            m_icon_image_id =
              static_cast<ImTextureID>(ImGui_ImplVulkan_AddTexture(
                m_icon_image.image().sampler(),
                m_icon_image.image().image_view(),
                VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL));
        }

        icon(const VkDevice& p_device,
             uint32_t p_memory_properties,
             const vk::image_extent& p_extent, std::span<const uint8_t> p_data) {
            m_extent = p_extent;
            m_icon_image = vk::texture(p_device, p_extent, p_data, p_memory_properties);
            m_icon_image_id =
              static_cast<ImTextureID>(ImGui_ImplVulkan_AddTexture(
                m_icon_image.image().sampler(),
                m_icon_image.image().image_view(),
                VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL));
        }

        ~icon() {
            // implicitly destroy
            // destroy();
        }

        [[nodiscard]] uint32_t width() const { return m_extent.width; }

        [[nodiscard]] uint32_t height() const { return m_extent.height; }

        [[nodiscard]] ImTextureID texture_id() const { return m_icon_image_id; }

        void destroy() { m_icon_image.destruct(); }

    private:
        vk::image_extent m_extent{};
        vk::texture m_icon_image;
        ImTextureID m_icon_image_id;
    };
};