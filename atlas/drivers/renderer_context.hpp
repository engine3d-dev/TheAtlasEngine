#pragma once
#include <string>
#include <core/core.hpp>
#include <drivers/vulkan-cpp/vk_swapchain.hpp>
#include <drivers/vulkan-cpp/vk_command_buffer.hpp>

namespace atlas {
    class render_context {
    public:
        virtual ~render_context() = default;

        void begin_frame(const vk::vk_command_buffer& p_current,
                         const vk::vk_swapchain& p_swapchain_handler,
                         const glm::mat4& p_proj_view) {
            return start_frame(p_current, p_swapchain_handler, p_proj_view);
        }

        void end_frame() { return post_frame(); }

        void set_background_color(const std::array<float, 4>& p_color) {
            return background_color(p_color);
        }

    private:
        virtual void start_frame(const vk::vk_command_buffer& p_current,
                                 const vk::vk_swapchain& p_swapchain_handler,
                                 const glm::mat4& p_proj_view) = 0;
        virtual void post_frame() = 0;

        virtual void background_color(const std::array<float, 4>& p_color) = 0;
    };

    scope<render_context> initialize_renderer(
      const vk::vk_swapchain& p_swapchain,
      const std::string& p_tag);
};
