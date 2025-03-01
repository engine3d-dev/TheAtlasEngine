#pragma once
#include <string>
#include <vector>
#include <drivers/shader.hpp>
#include <vulkan/vulkan_core.h>

namespace atlas::vk{
    /**
     * @name Vulkan Shader
     * @note Loads in the two shader modules (vertex and fragment shaders)
    */
    class vk_shader : public shader{
    public:
        vk_shader() = default;
        vk_shader(const std::string& p_vertex, const std::string& p_fragment, const shader_pipeline_config& p_config = {});
        virtual ~vk_shader(){
        }

        static shader_pipeline_config shader_configuration(uint32_t p_width, uint32_t p_height);

        VkPipeline get_graphics_pipeline() { return m_graphics_pipeline; }


    private:
        VkPipeline graphics_pipeline() override { return m_graphics_pipeline; }
    private:
        std::vector<VkVertexInputBindingDescription> get_vertex_input_bind_description();
        std::vector<VkVertexInputAttributeDescription> get_vertex_attribute_description();

        std::vector<char> read_file(const std::string& p_filepath);

        void initialize_graphics_pipeline(const std::string& p_vertex, const std::string& p_fragment, const shader_pipeline_config& p_config);
        void initialize_shader_module(const std::vector<char>& p_binary, VkShaderModule& p_shader_module);

    private:
        VkPipeline m_graphics_pipeline;
        VkShaderModule m_vertex_shader_module;
        VkShaderModule m_fragment_shader_module;
    };
};