#include <core/application_instance.hpp>
#include <drivers/shader.hpp>
#include <drivers/vulkan/shaders/vulkan_shader.hpp>

namespace atlas{
    ref<shader> shader::create(const std::string p_VertShader, const std::string& p_FragShader, const vk::shader_pipeline_config& p_Config){
        switch (application_instance::current_api()){
        case API::VULKAN: return create_ref<vk::vk_shader>(p_VertShader, p_FragShader, p_Config);
        default:
            break;
        }

        return nullptr;
    }

    VkPipeline shader::get_graphics_pipeline(){
        return graphics_pipeline();
    }
};