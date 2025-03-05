#include <core/application.hpp>
#include <core/engine_logger.hpp>
#include <drivers/renderer_backend.hpp>
#include <drivers/vulkan/vulkan_renderer.hpp>

namespace atlas {
    scope<render_context> render_context::initialize(const std::string& p_tag) {
        switch (application::current_api()) {
            case API::VULKAN:
                return create_scope<vk::vk_renderer>(p_tag);

            default:
                console_log_fatal("API specified was not supported!");
                return nullptr;
        }
        return nullptr;
    }

    uint32_t render_context::get_current_frame() {
        return vk::vk_renderer::get_current_command_buffer_index();
    }

    void render_context::begin() {
        return begin_frame();
    }

    void render_context::end() {
        return end_frame();
    }
}; // namespace atlas