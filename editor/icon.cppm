module;

#include <flecs.h>
#include <imgui.h>
#include <imgui/backends/imgui_impl_glfw.h>
#include <imgui/backends/imgui_impl_vulkan.h>
#include <filesystem>
#include <span>

export module editor:icon;

import atlas.core.utilities;
import vk;
import atlas.drivers.vulkan.stb_image;

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
        icon(/*NOLINT*/ std::shared_ptr<vk::device> p_device,
             uint32_t p_memory_properties,
             const std::filesystem::path& p_filename) {
            /*NOLINT*/ m_device = p_device;
            vk::texture_params config_texture = {
                // .memory_mask = physical_device.memory_properties(
                // vk::memory_property::host_visible_bit |
                // vk::memory_property::host_coherent_bit),
                .memory_mask = p_memory_properties,
            };

            atlas::stb_image image(p_filename.string(), config_texture);
            m_icon_image = vk::texture(*m_device, &image, config_texture);

            m_extent = image.extent();
            m_icon_image_id = ImGui_ImplVulkan_AddTexture(
              m_icon_image.image().sampler(),
              m_icon_image.image().image_view(),
              VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
        }

        icon(/*NOLINT*/ std::shared_ptr<vk::device> p_device,
             uint32_t p_memory_properties,
             const vk::image_extent& p_extent,
             std::span<const uint8_t> p_data) {
            /*NOLINT*/ m_device = p_device;
            m_extent = p_extent;
            m_icon_image =
              vk::texture(*m_device, p_extent, p_data, p_memory_properties);
            m_icon_image_id = ImGui_ImplVulkan_AddTexture(
              m_icon_image.image().sampler(),
              m_icon_image.image().image_view(),
              VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
        }

        ~icon() = default;

        [[nodiscard]] bool loaded() const { return m_icon_image.loaded(); }

        [[nodiscard]] uint32_t width() const { return m_extent.width; }

        [[nodiscard]] uint32_t height() const { return m_extent.height; }

        [[nodiscard]] VkDescriptorSet texture_id() const {
            return m_icon_image_id;
        }

        void destroy() { m_icon_image.destruct(); }

    private:
        std::shared_ptr<vk::device> m_device;
        vk::image_extent m_extent{};
        vk::texture m_icon_image;
        VkDescriptorSet m_icon_image_id;
    };
};