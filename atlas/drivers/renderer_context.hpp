#pragma once
#include <string>
#include <core/core.hpp>
#include <drivers/vulkan-cpp/vk_swapchain.hpp>

namespace atlas {
    class render_context {
    public:
        virtual ~render_context() = default;

        void begin_frame(const ::vk::command_buffer& p_current,
                         const window_settings& p_settings,
                         const VkRenderPass& p_renderpass,
                         const VkFramebuffer& p_framebuffer,
                         const glm::mat4& p_proj_view) {
            return start_frame(
              p_current, p_settings, p_renderpass, p_framebuffer, p_proj_view);
        }

        void end_frame() { return post_frame(); }

        void set_background_color(const std::array<float, 4>& p_color) {
            return background_color(p_color);
        }

        [[nodiscard]] ::vk::command_buffer get_active() const {
            return active_command();
        }

    private:
        virtual void start_frame(const ::vk::command_buffer& p_current,
                                 const window_settings& p_settings,
                                 const VkRenderPass& p_renderpass,
                                 const VkFramebuffer& p_framebuffer,
                                 const glm::mat4& p_proj_view) = 0;
        virtual void post_frame() = 0;

        virtual void background_color(const std::array<float, 4>& p_color) = 0;

        [[nodiscard]] virtual ::vk::command_buffer active_command() const = 0;
    };

    scope<render_context> initialize_renderer(
      const window_settings& p_window_extent,
      uint32_t p_image_size,
      const std::string& p_tag);
};
