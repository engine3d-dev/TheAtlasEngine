#include <Core/ApplicationInstance.hpp>
#include <Core/GraphicDrivers/Shader.hpp>
#include <internal/Vulkan2Showcase/Shaders/VulkanShader.hpp>

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