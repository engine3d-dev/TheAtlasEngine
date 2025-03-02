#include <core/application.hpp>
#include <core/engine_logger.hpp>
#include <drivers/renderer_backend.hpp>
#include <drivers/vulkan/vulkan_renderer.hpp>
#include <map>

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

    // void render_context::set_custom_shader(const std::string& p_Vert, const
    // std::string& p_Frag, bool p_IsOverride){ return CustomShaderImpl(p_Vert,
    // p_Frag, p_IsOverride);
    // }

    uint32_t render_context::get_current_frame() {
        return vk::vk_renderer::get_current_command_buffer_index();
    }

    void render_context::begin() { return begin_frame(); }

    void render_context::end() { return end_frame(); }

    void render_context::render_scene_objects(const ref<scene_scope>& p_scene) {
        return draw_scene_objects(p_scene);
    }

    void render_context::render_scene_object(ref<scene_object>& p_scene_object) {
        return draw_scene_object(p_scene_object);
    }

    void render_context::render_with_camera(ref<scene_object>& p_scene_object,
                                            ref<scene_object>& p_camera) {
        return draw_object_with_camera(p_scene_object, p_camera);
    }

    void function(int x_with_you_see, int y_you_see) {
        x_with_you_see += y_you_see;
        if (!x_with_you_see) {
        }
    }

}; // namespace atlas