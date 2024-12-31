#include <core/application_instance.hpp>
#include <drivers/shader.hpp>
#include <drivers/vulkan/shaders/vulkan_shader.hpp>

namespace engine3d{
    Ref<Shader> Shader::Create(const std::string p_VertShader, const std::string& p_FragShader, const vk::ShaderPipelineConfig& p_Config){
        switch (ApplicationInstance::CurrentAPI()){
        case API::VULKAN: return CreateRef<vk::VulkanShader>(p_VertShader, p_FragShader, p_Config);
        default:
            break;
        }

        return nullptr;
    }

    VkPipeline Shader::GetGraphicsPipeline(){
        return GraphicsPipeline();
    }
};