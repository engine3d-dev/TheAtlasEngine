#pragma once
#include <string>
#include <core/core.hpp>
#include <drivers/vulkan-cpp/vk_swapchain.hpp>
#include <core/scene/scene.hpp>

namespace atlas {
    /**
     * @brief is an interface that defines a graphics APi-agnostic renderer
     *
     * This context allows for communication from the the internal engine logic
     * to API-agnostic implementation of the renderers.
     *
     * This class is at the renderer-level of configurations. That relies on
     * atlas::graphics_context for setting up the agnostic-graphics API's
     * directly
     */
    class render_context {
    public:
        virtual ~render_context() = default;

        /**
         * @brief responsibility is to preload any data that is necessary to be
         * loaded before being rendered
         */
        void preload(const VkRenderPass& p_renderpass) {
            return preload_assets(p_renderpass);
        }

        /**
         * @brief indicator of when the start of the frame is
         *
         * Semantically is used to indicate this is when we start recording
         * operations to the GPU
         *
         * @param p_current is the current command buffer for recording
         * @param p_settings is the current window settings that are currently
         * applied
         * @param p_renderpass is the main renderpass for doing the rendering
         * operations
         * @param p_framebuffer is the framebuffer handle required and passed to
         * the renderpass
         * @param p_proj_view is the (proj * view) camera matrices that is used
         * by the game objects being rendered and passed as a shader uniform
         */
        void begin_frame(const ::vk::command_buffer& p_current,
                         const window_settings& p_settings,
                         const VkRenderPass& p_renderpass,
                         const VkFramebuffer& p_framebuffer,
                         const glm::mat4& p_proj_view) {
            return start_frame(
              p_current, p_settings, p_renderpass, p_framebuffer, p_proj_view);
        }

        /**
         * @brief Intended to use to indicate when to end recording to the GPU
         * in the current frame
         */
        void end_frame() { return post_frame(); }

        /**
         * @brief sets the background color and request that change to the
         * graphics API
         */
        void set_background_color(const std::array<float, 4>& p_color) {
            return background_color(p_color);
        }

        void current_scene_context(ref<scene> p_scene) {
            return current_scene(std::move(p_scene));
        }

    private:
        virtual void preload_assets(const VkRenderPass& p_renderpass) = 0;

        virtual void start_frame(const ::vk::command_buffer& p_current,
                                 const window_settings& p_settings,
                                 const VkRenderPass& p_renderpass,
                                 const VkFramebuffer& p_framebuffer,
                                 const glm::mat4& p_proj_view) = 0;
        virtual void post_frame() = 0;

        virtual void background_color(const std::array<float, 4>& p_color) = 0;

        virtual void current_scene(ref<scene>) = 0;
    };

    ref<render_context> initialize_renderer(
      const window_settings& p_window_extent,
      uint32_t p_image_size,
      const std::string& p_tag);
};
