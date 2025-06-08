#include <renderer/renderer.hpp>
#include <drivers/renderer_context.hpp>
#include <core/engine_logger.hpp>

namespace atlas {
    //! @note Only this renderer should have ownership of this renderer context
    //! @note Renderer context is  a graphics-agnostic renderer that provides an
    //! API to interact with our agnostic graphcis API's such as Vulkan, DirectX
    //! (if we plan to), etc.
    // scope<render_context> g_renderer_backend = nullptr;
    renderer* renderer::s_instance= nullptr;

    renderer::renderer(const vk::vk_swapchain& p_swapchain, const std::string& p_tag) {
        console_log_info("Begin Renderer Initialization!!!");
        m_render_context = initialize_renderer(p_swapchain, p_tag);

        console_log_info("End Renderer Initialization!!!\n\n");
    }

    void renderer::begin(const vk::vk_command_buffer& p_current, const vk::vk_swapchain& p_current_rp) {
        return m_render_context->begin_frame(p_current, p_current_rp);
    }

    void renderer::end() {
        return m_render_context->end_frame();
    }

    void renderer::set_background_color(const std::array<float, 4>& p_color) {
        m_render_context->set_background_color(p_color);
    }
};