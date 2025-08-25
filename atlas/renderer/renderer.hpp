#pragma once
#include <core/core.hpp>
#include <vulkan/vulkan.hpp>
#include <drivers/renderer_context.hpp>
#include <drivers/vulkan-cpp/vk_swapchain.hpp>

namespace atlas {
    /**
     * @name Renderer
     * @brief Submits high-level tasks that may get executed based on
     * specifications for workloads
     * @brief Provides API's for direct immediate execution to the backend
     * context
     * @brief Direct API's for performing certain rendering optimizations
     * implementations
     */
    class renderer {
    public:
        renderer() = default;
        renderer(const vk::vk_swapchain& p_swapchain,
                 const std::string& p_tag = "Renderer");
        /**
         * @brief Indicates to the renderer is at the start of the next frame to
         * prepare workloads before next frame is processeed
         */
        void begin(const vk::vk_command_buffer& p_current,
                   const vk::vk_swapchain& p_current_rp,
                   const glm::mat4& p_proj_view);

        /**
         * @brief Indications when the renderer has reached the end of the frame
         */
        void end();

        void present(uint32_t p_frame_index);

        void set_background_color(const std::array<float, 4>& p_color);

    private:
        static renderer* s_instance;
        scope<render_context> m_render_context = nullptr;
    };
};
