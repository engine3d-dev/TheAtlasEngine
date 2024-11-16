#pragma once
#include <string>
#include <vector>
#include <vulkan/vulkan_core.h>
#include <array>

namespace engine3d::vk{
    /**
     * @name Vulkan Shader
     * @note Loads in the two shader modules (vertex and fragment shaders)
    */
    class VulkanShader{
        //! @note Used for specifying how the shader pipeline configuration may look like.
        struct ShaderPipelineConfig{
            VkViewport Viewport;
            VkRect2D Scissor;
            // VkPipelineViewportStateCreateInfo PipelineViewportCreateInfo;
            VkPipelineInputAssemblyStateCreateInfo PipelineInputAsmInfo;
            VkPipelineRasterizationStateCreateInfo PipelineRasterizationCreateInfo;
            VkPipelineMultisampleStateCreateInfo PipelineMultisampleCreateInfo;
            VkPipelineColorBlendAttachmentState PipelineColorBlendAttachments;
            VkPipelineColorBlendStateCreateInfo PipelineColorBlendCreateInfo;
            VkPipelineDepthStencilStateCreateInfo PipelineDepthStencilCreateInfo;
            VkPipelineLayout PipelineLayout;
            VkRenderPass PipelineRenderPass;
            uint32_t SubpassCount;
        };
    public:
        VulkanShader() = default;
        VulkanShader(const std::string& p_VertShader, const std::string& p_FragShader, const ShaderPipelineConfig& p_Config = {});
        ~VulkanShader(){}

        static VulkanShader::ShaderPipelineConfig shader_configuration(uint32_t p_Width, uint32_t p_Height);

        VkPipeline GetGraphicsPipeline() { return m_GraphicsPipeline; }

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