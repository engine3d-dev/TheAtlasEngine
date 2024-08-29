#pragma once
#include <Core/backend/Vulkan/Shaders/VulkanShaderPipelineBuilder.h>
#include <filesystem>

namespace engine3d{
    namespace vk{
        class VulkanShader{
        public:
            //! @note Loading shaders separately
            VulkanShader(const std::string& vertFilename, const std::string& fragFilename);
            // VulkanShader(const VulkanShaderPipelineBuilder& pipeline);

            // static VulkanShader Load(const std::string& vertFilename);

            bool IsLoaded();
        private:
            bool m_IsShaderLoaded;
            VulkanShaderModule m_VertShaderModule;
            VulkanShaderModule m_FragShaderModule;
            VulkanShaderPipelineBuilder m_ShaderPipeline;
        };
    };
};