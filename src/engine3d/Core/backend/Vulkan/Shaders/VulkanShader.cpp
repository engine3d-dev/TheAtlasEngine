#include <Core/backend/Vulkan/Shaders/VulkanShader.h>

namespace engine3d::vk{
    VulkanShader::VulkanShader(const std::string& vertFilename, const std::string& fragFilename){
        m_VertShaderModule = VulkanShaderModule(vertFilename);
        m_FragShaderModule = VulkanShaderModule(fragFilename);
        // m_ShaderPipeline = VulkanShaderPipelineBuilder(m_VertShaderModule, m_FragShaderModule);
        m_IsShaderLoaded = false;
    }

    //! @note Both vertex and fragment shaders must be loaded successful
    bool VulkanShader::IsLoaded(){
        return m_VertShaderModule.IsLoaded() and m_FragShaderModule.IsLoaded();
    }
};