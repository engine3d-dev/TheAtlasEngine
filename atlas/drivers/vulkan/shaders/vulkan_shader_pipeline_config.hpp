#pragma once
#include <vulkan/vulkan.h>

namespace atlas::vk{
    //! @note Used for specifying how the shader pipeline configuration may look like.
    //! @note Used by vk_shader
    struct shader_pipeline_config{
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
};