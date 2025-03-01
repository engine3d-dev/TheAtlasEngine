#pragma once
#include <core/core.hpp>
#include <vulkan/vulkan.h>
#include <string>
#include <drivers/vulkan/shaders/vulkan_shader_pipeline_config.hpp>

namespace atlas{
    class shader{
    public:
        virtual ~shader() = default;
        static ref<shader> create(const std::string p_VertShader, const std::string& p_FragShader, const vk::shader_pipeline_config& p_Config);

        VkPipeline get_graphics_pipeline();

    private:
        virtual VkPipeline graphics_pipeline() = 0;

    };
};