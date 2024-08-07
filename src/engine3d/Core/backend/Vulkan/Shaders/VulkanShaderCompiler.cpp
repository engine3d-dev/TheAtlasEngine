#include <Core/EngineLogger.h>
#include <Core/backend/Vulkan/Vulkan.h>
#include <Core/backend/Vulkan/VulkanDevice.h>
#include "Core/backend/utilities/helper_functions.h"
#include <Core/backend/Vulkan/Shaders/VulkanShaderCompiler.h>
#include <vulkan/vulkan_core.h>

namespace engine3d::vk{
    /**
     * @param pipeline_shader_create_info
     * @note Contains information about our shader stage for our pipeline.
     * @note For when building from shader stage and shader module.
    */
    static VkPipelineShaderStageCreateInfo pipeline_shader_create_info(VkShaderStageFlagBits stage, VkShaderModule shaderModule){
        VkPipelineShaderStageCreateInfo createInfo{
            .sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
            .pNext = nullptr,
            .stage = stage,
            .module = shaderModule,
            .pName = "main"
        };

        return createInfo;
    }

    /**
     * @param vertex_input_state_create_info
     * @note Handles informationabout vertex buffers and vertex formats
     * @note Equivalent to configurations VAO in OpenGL
     * @note For now we'll initiate vertex buffers like this.
    */
    static VkPipelineVertexInputStateCreateInfo  vertex_input_state_create_info(){
        VkPipelineVertexInputStateCreateInfo createInfo{
            .sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO,
            .pNext = nullptr,
            .vertexBindingDescriptionCount = 0,
            .vertexAttributeDescriptionCount = 0
        };

        return createInfo;
    }

    /**
     * @param input_assembly_create_info
     * @param VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST - normal triangle drawing
     * @param VK_PRIMITIVE_TOPOLOGY_POINT_LIST - points
     * @param VK_PRIMITIVE_TOPOLOGY_LINE_LIST - line-list
     * @note Information on what kinds of topology will be drawn
     * @note This is where you set it to draw triangles, lines, points, or other triangle-like lists.
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

    /**
     * @param rasterization_state_create_info
     * @note configures the fixed-function rasterization
     * @note In this function we enable/disable backface culling and set line width or wireframe drawing.
     * @note Setting polygonMode as editable inpuit to be able toggle between wireframes/solidframes.
     * @note cujllMode - uses to cull backfaces/frontfaces.
     * @note rasterizerDiscardEnable is enable, primitives (triangles in this example) are discarded before even making it to the rasterization stage which means the triangles would never get drawn to the screen.
    */
    static VkPipelineRasterizationStateCreateInfo rasterization_state_create_info(VkPolygonMode polygonMode){
        VkPipelineRasterizationStateCreateInfo createInfo = {
            .sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO,
            .pNext = nullptr,
            .depthClampEnable = VK_FALSE,
            .rasterizerDiscardEnable = VK_FALSE,

            .polygonMode = polygonMode,
            .cullMode = VK_CULL_MODE_NONE,
            .frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE,
            .depthBiasEnable = VK_FALSE,
            .depthBiasConstantFactor = 0.0f,
            .depthBiasClamp = 0.0f,
            .depthBiasSlopeFactor = 0.0f
        };

        return createInfo;
    }

    /**
     * @param multisample_state_create_info
     * @note 
    */
    static VkPipelineMultisampleStateCreateInfo multisample_state_create_info(){
        VkPipelineMultisampleStateCreateInfo createInfo{
            .sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO,
            .pNext = nullptr,
            .sampleShadingEnable = VK_FALSE,
            .rasterizationSamples = VK_SAMPLE_COUNT_1_BIT,
            .minSampleShading = 1.0f,
            .pSampleMask = nullptr,
            .alphaToCoverageEnable = VK_FALSE,
            .alphaToOneEnable = VK_FALSE
        };

        return createInfo;
    }

    static VkPipelineColorBlendAttachmentState color_blend_attachment_state(){
        VkPipelineColorBlendAttachmentState colorBlendAttachment{
            .colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT,
            .blendEnable = VK_FALSE
        };

        return colorBlendAttachment;
    }

    static VkPipelineLayoutCreateInfo pipeline_layout_create_info(){
        VkPipelineLayoutCreateInfo createInfo = {
            .sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
            .pNext = nullptr,

            // empty defaults
            .flags = 0,
            .setLayoutCount = 0,
            .pSetLayouts = nullptr,
            .pushConstantRangeCount = 0,
            .pPushConstantRanges = nullptr
        };

        return createInfo;
    }

    VkPipeline VulkanShaderCompiler::make_pipeline(VkRenderPass pass){

        //! @note Making the viewport state from our stored viewport and scissor
        VkPipelineViewportStateCreateInfo viewportStateCreateInfo = {
            .sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO,
            .pNext = nullptr,

            .viewportCount = 1,
            .pViewports = &m_Viewport,
            .scissorCount = 1,
            .pScissors = &m_Scissor
        };

        VkPipelineColorBlendStateCreateInfo colorBlending = {
            .sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO,
            .pNext = nullptr,
            .logicOpEnable = VK_FALSE,
            .logicOp = VK_LOGIC_OP_COPY,
            .attachmentCount = 1,
            .pAttachments = &m_ColorBlendAttachment
        };

        VkGraphicsPipelineCreateInfo pipelineInfo = {
            .sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO,
            .pNext = nullptr,

            .stageCount = (uint32_t)m_ShaderStages.size(),
            .pStages = m_ShaderStages.data(),
            .pVertexInputState = &m_VertexCreateInfo,
            .pInputAssemblyState = &m_InputAssemblyCreateInfo,
            .pViewportState = &viewportStateCreateInfo,
            .pRasterizationState = &m_Rasterizer,
            .pMultisampleState = &m_Multisample,
            .pColorBlendState = &colorBlending,
            .layout = m_PipelineLayout,
            .renderPass = pass,
            .subpass = 0,
            .basePipelineHandle = VK_NULL_HANDLE
        };

        VkPipeline newPipeline;

        VkResult res = vkCreateGraphicsPipelines(VulkanDevice::GetVkLogicalDeviceInstance(), VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &newPipeline);

        if(res != VK_SUCCESS){
            ConsoleLogError("vkCreateGraphicsPipelines errored out and gave this following error message\t\t{}", VkResultToString(res));
            return VK_NULL_HANDLE;
        }

        return newPipeline;
    }

    VulkanShaderCompiler::VulkanShaderCompiler(VkShaderModule triangleVertShader, VkShaderModule triangleFragShader, VkRenderPass pass){
        VkPipelineLayoutCreateInfo pipelineCreateInfo = pipeline_layout_create_info();
        VkResult res = vkCreatePipelineLayout(VulkanDevice::GetVkLogicalDeviceInstance(), &pipelineCreateInfo, nullptr, &m_TrianglePipelineLayout);

        if(res != VK_SUCCESS){
            ConsoleLogError("vkCreatePipelineLayout errored and the error message sent was\t\t{}", VkResultToString(res));
        }

        // Creating our pipeline builder here using the make_pipeline function
        //! @note And utilizing the shader modules that we have created!
        //! @note Also how we can load in our vertex and fragment shaders here
        m_ShaderStages.push_back(pipeline_shader_create_info(VK_SHADER_STAGE_VERTEX_BIT, triangleVertShader));
        m_ShaderStages.push_back(pipeline_shader_create_info(VK_SHADER_STAGE_FRAGMENT_BIT, triangleFragShader));

        //! @note Vertex input controls reading vertices from vertex buffers. Not using it atm.
        m_VertexCreateInfo = vertex_input_state_create_info();

        //! @note Input asm is configurations for drawing triangle lists, strips, or individual points.
        m_InputAssemblyCreateInfo = input_assembly_create_info(VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST);

        //! @note building viewport/scissor from the swapchain extents.
        m_Viewport.x = 0.0f;
        m_Viewport.x = 0.0f;
        m_Viewport.width = (float)VulkanPipeline::GetWidth();
        m_Viewport.height = (float)VulkanPipeline::GetHeight();
        m_Viewport.minDepth = 0.0f;
        m_Viewport.maxDepth = 1.0f;

        m_Scissor.offset = {0, 0};
        m_Scissor.extent = {VulkanPipeline::GetWidth(), VulkanPipeline::GetHeight()};

        // configuring rasterizser
        m_Rasterizer = rasterization_state_create_info(VK_POLYGON_MODE_FILL);

        // not using the multisampler right now, using default multisampler
        m_Multisample = multisample_state_create_info();

        // a single blend attachment with no blending and writing to RGBA.
        m_ColorBlendAttachment = color_blend_attachment_state();

        //! @note Use triangle layout we created
        m_PipelineLayout = m_TrianglePipelineLayout;

        // finally building the actual pipeline
        m_TrianglePipeline = make_pipeline(pass);

        ConsoleLogInfo("VulkanShaderPipeline initiated successful!");
    }

    VkPipeline& VulkanShaderCompiler::GetShaderPipeline(){ return m_TrianglePipeline; }
};