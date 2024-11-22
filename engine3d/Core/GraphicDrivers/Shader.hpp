#pragma once
#include <Core/Core.hpp>
#include <vulkan/vulkan.h>
#include <Core/internal/Vulkan2Showcase/Shaders/ShaderPipelineConfig.hpp>

namespace engine3d{
    class Shader{
    public:
        static Ref<Shader> Create(const std::string p_VertShader, const std::string& p_FragShader, const vk::ShaderPipelineConfig& p_Config);

        VkPipeline GetGraphicsPipeline();

    private:
        virtual VkPipeline GraphicsPipeline() = 0;

    };
};