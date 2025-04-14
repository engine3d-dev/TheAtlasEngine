#include <core/application.hpp>
#include <drivers/shader.hpp>
#include <drivers/vulkan/shaders/vulkan_shader.hpp>

namespace atlas {
    ref<shader> shader::create(const std::string& p_vert_shader,
                               const std::string& p_frag_shader,
                               const vk::shader_pipeline_config& p_config) {
        switch (application::current_api()) {
            case API::VULKAN:
                return create_ref<vk::vk_shader>(
                  p_vert_shader, p_frag_shader, p_config);
            default:
                break;
        }

        return nullptr;
    }

    VkPipeline shader::get_graphics_pipeline() {
        return graphics_pipeline();
    }
};