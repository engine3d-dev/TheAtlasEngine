#pragma once
#include <vector>
#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>

namespace engine3d{
    namespace vk{
        class VulkanShaderPipelineBuilder{
        public:
            VulkanShaderPipelineBuilder() = default;
            VulkanShaderPipelineBuilder(VkShaderModule vertShaderModule, VkShaderModule fragShaderModule, VkRenderPass& pass);

            VkPipeline& GetVkPipeline();

        private:
            VkShaderModule m_VertShaderModule;
            VkShaderModule m_FragShaderModule;

            // pipeline builder properties
            std::vector<VkPipelineShaderStageCreateInfo> m_ShaderStages;
            VkPipelineVertexInputStateCreateInfo m_VertexInputInfo;
            VkPipelineInputAssemblyStateCreateInfo m_InputAssembly;
            VkViewport m_Viewport;
            VkRect2D m_Scissor;
            VkPipelineRasterizationStateCreateInfo m_Rasterizer;
            VkPipelineColorBlendAttachmentState m_ColorBlendAttachment;
            VkPipelineMultisampleStateCreateInfo m_Multisampling;
            
            VkPipelineLayout m_PipelineLayout;
            VkPipeline m_Pipeline;

            VkPipeline make_pipeline(VkDevice dev, VkRenderPass& pass);
        };
    };
};