#pragma once
#include <vulkan/vulkan.h>

namespace engine3d::vk{
    //! @note Used for specifying how the shader pipeline configuration may look like.
    //! @note Used by VUlkanShader
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
};