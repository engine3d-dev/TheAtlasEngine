#pragma once
#include <core/core.hpp>
#include <drivers/vulkan/shaders/vulkan_shader_pipeline_config.hpp>
#include <string>
#include <vulkan/vulkan.h>

namespace atlas {
    class shader {
    public:
        virtual ~shader() = default;
        static ref<shader> create(const std::string& p_vert_shader,
                                  const std::string& p_frag_shader,
                                  const vk::shader_pipeline_config& p_config);

        VkPipeline get_graphics_pipeline();

    private:
        virtual VkPipeline graphics_pipeline() = 0;
    };
};