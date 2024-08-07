#pragma once
#include <vulkan/vulkan.h>
#include <vector>
#include <string>
#include <vulkan/vulkan_core.h>

namespace engine3d{
    namespace vk{
        /**
         * @name VulkanShaderCompiler
         * @note Represent our VkPipeline for configuring entire GPU for drawing.
         * @note Once pipelines built, it can be bounded inside cmd buffer and when your draw anything, it'll use bounded pipeline.
         * @note Vulkan pipelines are massive objects with different configuration structs, some with even running pointers/being arrays
         * @note This class will help abstract this process of VkPipeline.
        */
        class VulkanShaderCompiler{
        public:
            VulkanShaderCompiler() = default;
            VulkanShaderCompiler(VkShaderModule triangleVertShader, VkShaderModule triangleFragShader, VkRenderPass pass);

            VkPipeline make_pipeline(VkRenderPass pass);
            void InitiateShaderPipeline();

            VkPipeline& GetShaderPipeline();
        private:
            std::vector<VkPipelineShaderStageCreateInfo> m_ShaderStages;
            VkPipelineVertexInputStateCreateInfo m_VertexCreateInfo;
            VkPipelineInputAssemblyStateCreateInfo m_InputAssemblyCreateInfo;
            VkViewport m_Viewport;
            VkRect2D m_Scissor;
            VkPipelineRasterizationStateCreateInfo m_Rasterizer;
            VkPipelineColorBlendAttachmentState m_ColorBlendAttachment;
            VkPipelineMultisampleStateCreateInfo m_Multisample;
            VkPipelineLayout m_PipelineLayout;
            VkPipeline m_TrianglePipeline;
            VkPipelineLayout m_TrianglePipelineLayout;
            
        };
    };
};