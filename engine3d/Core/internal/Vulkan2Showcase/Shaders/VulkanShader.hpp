#pragma once
#include <string>
#include <vector>
#include <Core/GraphicDrivers/Shader.hpp>
#include <vulkan/vulkan_core.h>
// #include <internal/Vulkan2Showcase/Shaders/ShaderPipelineConfig.hpp>

namespace engine3d::vk{
    /**
     * @name Vulkan Shader
     * @note Loads in the two shader modules (vertex and fragment shaders)
    */
    class VulkanShader : public Shader{
    public:
        VulkanShader() = default;
        VulkanShader(const std::string& p_VertShader, const std::string& p_FragShader, const ShaderPipelineConfig& p_Config = {});
        ~VulkanShader(){}

        static ShaderPipelineConfig shader_configuration(uint32_t p_Width, uint32_t p_Height);

        VkPipeline GetGraphicsPipeline() { return m_GraphicsPipeline; }


    private:
        VkPipeline GraphicsPipeline() override { return m_GraphicsPipeline; }
    private:

        std::vector<char> read_file(const std::string& p_Filepath);

        void initialize_graphics_pipeline(const std::string& p_VertShader, const std::string& p_FragShader, const ShaderPipelineConfig& p_Config);
        void initialize_shader_module(const std::vector<char>& p_Bin, VkShaderModule& p_ShaderMod);

    private:
        VkPipeline m_GraphicsPipeline;
        VkShaderModule m_VertexShaderModule;
        VkShaderModule m_FragmentShaderModule;
    };
};