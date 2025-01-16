#pragma once
#include <core/core.hpp>
#include <vulkan/vulkan.h>
#include <string>
#include <drivers/vulkan/shaders/vulkan_shader_pipeline_config.hpp>

namespace engine3d{
    class Shader{
    public:
        virtual ~Shader() = default;
        static Ref<Shader> Create(const std::string p_VertShader, const std::string& p_FragShader, const vk::ShaderPipelineConfig& p_Config);

        VkPipeline GetGraphicsPipeline();

    private:
        virtual VkPipeline GraphicsPipeline() = 0;

    };
};