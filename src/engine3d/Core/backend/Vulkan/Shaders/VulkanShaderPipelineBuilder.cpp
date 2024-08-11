#include "Core/EngineLogger.h"
#include "Core/backend/Vulkan/Vulkan.h"
#include "Core/backend/Vulkan/VulkanDevice.h"
#include "Core/backend/utilities/helper_functions.h"
#include <Core/backend/Vulkan/Shaders/VulkanShaderPipelineBuilder.h>
#include <vulkan/vulkan_core.h>

namespace engine3d::vk{
    
    //! @note TODO --- Deal with these laterr... meaning that I'll do a refactor on how these are organized within our implementation for the shader pipeline.
    //! @note Contains information about our shader stage
    static VkPipelineShaderStageCreateInfo pipeline_shader_stage_create_info(VkShaderStageFlagBits stage, VkShaderModule shaderModule){
        VkPipelineShaderStageCreateInfo createInfo = {
            .sType =VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
            .pNext = nullptr,
            .stage = stage,
            .module = shaderModule,
            .pName = "main"
        };

        return createInfo;
    }

    //! @note Vertex Input State
    static VkPipelineVertexInputStateCreateInfo vertex_input_state_create_info(){
        VkPipelineVertexInputStateCreateInfo createInfo = {
            .sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO,
            .pNext = nullptr,

            .vertexBindingDescriptionCount = 0,
            .vertexAttributeDescriptionCount = 0
        };

        return createInfo;
    }

    //! @note Input assembly
    /**
     * @note TODO -- Should have a engine3d-specific enum to indicate what type of draw command we want. Whether it is a triangle, point, or line lists
    *  @param VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST : normal triangle drawing
    *  @param VK_PRIMITIVE_TOPOLOGY_POINT_LIST : points
    *  @param VK_PRIMITIVE_TOPOLOGY_LINE_LIST : line-list
    */
    static VkPipelineInputAssemblyStateCreateInfo input_assembly_create_info(VkPrimitiveTopology topology){
        VkPipelineInputAssemblyStateCreateInfo createInfo = {
            .sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO,
            .pNext = nullptr,
            .topology = topology,
            .primitiveRestartEnable = VK_FALSE
        };

        return createInfo;
    }

    //! @note Rasterization State
    //! @note Configurations for fixed function rasterization. Where we enable/disable backface culling/set line width or wireframe drawing
    static VkPipelineRasterizationStateCreateInfo rasterization_state_create_info(VkPolygonMode polygonMode){
        VkPipelineRasterizationStateCreateInfo createInfo = {
            .sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO,
            .pNext = nullptr,
            
            .depthClampEnable = VK_FALSE,
            .rasterizerDiscardEnable = VK_FALSE,

            .polygonMode = polygonMode,
            .cullMode = VK_CULL_MODE_NONE,
            .frontFace = VK_FRONT_FACE_CLOCKWISE,
            .lineWidth = 1.0f,
            .depthBiasEnable = VK_FALSE,
            .depthBiasConstantFactor = 0.0f,
            .depthBiasClamp = 0.0F,
            .depthBiasSlopeFactor = 0.0f
        };

        return createInfo;
    }

    //! @note Multisampling state
    static VkPipelineMultisampleStateCreateInfo multisampling_state_create_info(){
        VkPipelineMultisampleStateCreateInfo createInfo = {
            .sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO,
            .pNext = nullptr,

            .rasterizationSamples = VK_SAMPLE_COUNT_1_BIT,
            .sampleShadingEnable = VK_FALSE,
            .minSampleShading = 1.0f,
            .pSampleMask = nullptr,
            .alphaToCoverageEnable = VK_FALSE,
            .alphaToOneEnable = VK_FALSE
        };

        return createInfo;
    }

    //! @note Color blend attachment state
    static VkPipelineColorBlendAttachmentState color_blend_attachment_state(){
        VkPipelineColorBlendAttachmentState colorBlendAttachment = {};
		colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT |
			VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
		colorBlendAttachment.blendEnable = VK_FALSE;
		return colorBlendAttachment;
    }

    VkPipeline VulkanShaderPipelineBuilder::make_pipeline(VkDevice logicalDevice, VkRenderPass& pass){
        VkPipelineViewportStateCreateInfo viewportCreateInfo = {
            .sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO,
            .pNext = nullptr,

            .viewportCount = 1,
            .pViewports = &m_Viewport,
            
            .scissorCount = 1,
            .pScissors = &m_Scissor,
        };

        VkPipelineColorBlendStateCreateInfo colorBlendCreateInfo = {
            .sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO,
            .pNext = nullptr,

            .logicOpEnable = VK_FALSE,
            .logicOp = VK_LOGIC_OP_COPY,
            .attachmentCount = 1,
            .pAttachments = &m_ColorBlendAttachment
        };

        VkGraphicsPipelineCreateInfo graphicsPipelineCreateInfo = {
            .sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO,
            .pNext = nullptr,
            .stageCount = (uint32_t)m_ShaderStages.size(),
            .pStages = m_ShaderStages.data(),
            .pVertexInputState = &m_VertexInputInfo,
            .pInputAssemblyState = &m_InputAssembly,
            .pViewportState = &viewportCreateInfo,
            .pRasterizationState = &m_Rasterizer,
            .pMultisampleState = &m_Multisampling,
            .pColorBlendState = &colorBlendCreateInfo,
            .layout = m_PipelineLayout,
            .renderPass = pass,
            .subpass = 0,
            .basePipelineHandle = VK_NULL_HANDLE
        };

        VkPipeline newPipeline;
        VkResult res = vkCreateGraphicsPipelines(logicalDevice, VK_NULL_HANDLE, 1, &graphicsPipelineCreateInfo, nullptr, &newPipeline);
        
        if(res != VK_SUCCESS){
            ConsoleLogError("vkCreateGraphicsPipelines errored message\t\t{}", VkResultToString(res));
            return VK_NULL_HANDLE;
        }

        return newPipeline;
    }

    VkPipelineLayout make_pipeline_layout(VkDevice logical){
        VkPipelineLayoutCreateInfo info{
            .sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
            .pNext = nullptr,

            .flags = 0,
            .setLayoutCount = 0,
            .pSetLayouts = nullptr,
            .pushConstantRangeCount = 0,
            .pPushConstantRanges = nullptr,
        };

        // return info;
        VkPipelineLayout layout;
        VkResult res = vkCreatePipelineLayout(logical, &info, nullptr, &layout);

        if(res != VK_SUCCESS){
            ConsoleLogError("vkCreatePipelineLayout errored message was\t\t{}", VkResultToString(res));
        }

        return layout;
    }



    VulkanShaderPipelineBuilder::VulkanShaderPipelineBuilder(VkShaderModule vertShaderModule, VkShaderModule fragShaderModule, VkRenderPass& pass){
        m_ShaderStages.push_back(pipeline_shader_stage_create_info(VK_SHADER_STAGE_VERTEX_BIT, vertShaderModule));
        m_ShaderStages.push_back(pipeline_shader_stage_create_info(VK_SHADER_STAGE_FRAGMENT_BIT, fragShaderModule));

        m_VertexInputInfo = vertex_input_state_create_info();
        m_InputAssembly = input_assembly_create_info(VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST);

        // m_Viewport.x = 0.0f;
        // m_Viewport.y = 0.0f;
        // m_Viewport.width = VulkanPipeline::GetWidth()
        // m_Viewport.height = 0.0f;
        m_Viewport = {
            .x = 0.0f, .y = 0.0f,
            .width = (float)VulkanPipeline::GetWidth(),
            .height = (float)VulkanPipeline::GetHeight(),
            .minDepth = 0.0f,
            .maxDepth = 1.0f
        };

        m_Scissor = {
            .offset = {0, 0},
            .extent = {VulkanPipeline::GetWidth(), VulkanPipeline::GetHeight()}
        };

        m_Rasterizer = rasterization_state_create_info(VK_POLYGON_MODE_FILL);

        m_Multisampling = multisampling_state_create_info();

        m_ColorBlendAttachment = color_blend_attachment_state();

        m_PipelineLayout = make_pipeline_layout(VulkanDevice::GetVkLogicalDeviceInstance());

        m_Pipeline = make_pipeline(VulkanDevice::GetVkLogicalDeviceInstance(), pass);
        m_VertShaderModule = vertShaderModule;
        m_FragShaderModule = fragShaderModule;

        ConsoleLogInfo("VulkanShaderPipelineBuilder finished loading shader pipeline!");
    }

    VkPipeline& VulkanShaderPipelineBuilder::GetVkPipeline(){
        return m_Pipeline;
    }
};