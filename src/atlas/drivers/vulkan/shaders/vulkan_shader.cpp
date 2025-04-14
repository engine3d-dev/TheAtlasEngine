#include <core/engine_logger.hpp>
#include <drivers/vertex_buffer.hpp>
#include <drivers/vulkan/helper_functions.hpp>
#include <drivers/vulkan/shaders/vulkan_shader.hpp>
#include <drivers/vulkan/vulkan_context.hpp>
#include <fstream>
#include <vulkan/vulkan_core.h>

namespace atlas::vk {
    vk_shader::vk_shader(const std::string& p_vert_shader,
                         const std::string& p_frag_shader,
                         const shader_pipeline_config& p_config) {
        initialize_graphics_pipeline(p_vert_shader, p_frag_shader, p_config);
    }

    std::vector<char> vk_shader::read_file(const std::string& p_filepath) {
        std::ifstream ins(p_filepath, std::ios::ate | std::ios::binary);

        if (!ins) {
            console_log_error("Could not read in file \"{}\"", p_filepath);
            return {};
        }
        uint32_t file_size = static_cast<uint32_t>(ins.tellg());
        std::vector<char> buffer(file_size);

        ins.seekg(0);
        ins.read(buffer.data(), file_size);
        return buffer;
    }

    void vk_shader::initialize_graphics_pipeline(
      const std::string& p_vert_shader,
      const std::string& p_frag_shader,
      const shader_pipeline_config& p_config) {
        auto vert = read_file(p_vert_shader);
        auto frag = read_file(p_frag_shader);

        initialize_shader_module(vert, m_vertex_shader_module);
        initialize_shader_module(frag, m_fragment_shader_module);

        //! @note Setting vertex/fragment shaders stages.

        std::array<VkPipelineShaderStageCreateInfo, 2> shader_stages;

        VkPipelineShaderStageCreateInfo vert_shader_stage = {
            .sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
            .pNext = nullptr,
            .flags = 0,
            .stage = VK_SHADER_STAGE_VERTEX_BIT,
            .module = m_vertex_shader_module,
            .pName = "main"
        };

        VkPipelineShaderStageCreateInfo frag_shader_stage = {
            .sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
            .pNext = nullptr,
            .flags = 0,
            .stage = VK_SHADER_STAGE_FRAGMENT_BIT,
            .module = m_fragment_shader_module,
            .pName = "main"
        };

        shader_stages[0] = vert_shader_stage;
        shader_stages[1] = frag_shader_stage;

        auto binding_description = get_vertex_input_bind_description();
        auto attachment_description = get_vertex_attribute_description();

        VkPipelineVertexInputStateCreateInfo vert_input_create_info = {
            .sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO,
            .pNext = nullptr,
            .vertexBindingDescriptionCount =
              static_cast<uint32_t>(binding_description.size()),
            .pVertexBindingDescriptions = binding_description.data(),
            .vertexAttributeDescriptionCount =
              static_cast<uint32_t>(attachment_description.size()),
            .pVertexAttributeDescriptions = attachment_description.data(),
        };

        VkPipelineViewportStateCreateInfo pipeline_viewport_create_info = {
            .sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO,
            .pNext = nullptr,
            .flags = 0,
            .viewportCount = 1,
            .pViewports = &p_config.Viewport,
            .scissorCount = 1,
            .pScissors = &p_config.Scissor
        };

        //! @note Setting the state of our viewport to being enabled to be in
        //! dynamic state
        //! @note For resizability
        std::array<VkDynamicState, 2> dynamic_states = {
            VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR
        };
        VkPipelineDynamicStateCreateInfo dynamic_state_ci = {
            .sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO,
            .dynamicStateCount = (uint32_t)dynamic_states.size(),
            .pDynamicStates = dynamic_states.data()
        };

        VkGraphicsPipelineCreateInfo graphics_pipeline_create_info = {
            .sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO,
            .stageCount = 2,
            .pStages = shader_stages.data(),
            .pVertexInputState = &vert_input_create_info,
            .pInputAssemblyState = &p_config.PipelineInputAsmInfo,
            .pViewportState = &pipeline_viewport_create_info,
            .pRasterizationState = &p_config.PipelineRasterizationCreateInfo,
            .pMultisampleState = &p_config.PipelineMultisampleCreateInfo,
            .pDepthStencilState = &p_config.PipelineDepthStencilCreateInfo,
            .pColorBlendState = &p_config.PipelineColorBlendCreateInfo,
            // .pDynamicState = nullptr,
            .pDynamicState = &dynamic_state_ci,
            .layout = p_config.PipelineLayout,
            .renderPass = p_config.PipelineRenderPass,
            .subpass = p_config.SubpassCount,
            .basePipelineHandle = nullptr,
            .basePipelineIndex = -1,
        };

        vk_check(vkCreateGraphicsPipelines(vk_context::get_current_driver(),
                                           nullptr,
                                           1,
                                           &graphics_pipeline_create_info,
                                           nullptr,
                                           &m_graphics_pipeline),
                 "vkCreateGraphicsPipelines",
                 __FILE__,
                 __LINE__,
                 __FUNCTION__);

        console_log_warn("vertex Shader Size == {}", vert.size());
        console_log_warn("Fragment Shader Size == {}", frag.size());
    }

    std::vector<VkVertexInputBindingDescription>
    vk_shader::get_vertex_input_bind_description() {
        std::vector<VkVertexInputBindingDescription> binding_descriptions(1);
        binding_descriptions[0] = { .binding = 0,
                                    .stride = sizeof(vertex),
                                    .inputRate = VK_VERTEX_INPUT_RATE_VERTEX };

        return binding_descriptions;
    }

    /*
        Replace this with

        VertexAttribute attributes = {
            {FLOAT, offsetof(vertex, Position)},
            {FLOAT, offsetof(vertex, Color)},
            {FLOAT, offsetof(vertex, Normals)},
            {FLOAT, offsetof(vertex, Uv)}
        };
    */
    std::vector<VkVertexInputAttributeDescription>
    vk_shader::get_vertex_attribute_description() {
        std::vector<VkVertexInputAttributeDescription> attribute_description{};

        attribute_description.push_back(
          { .location = 0,
            .binding = 0,
            .format = VK_FORMAT_R32G32B32_SFLOAT,
            .offset = offsetof(vertex, Position) });
        attribute_description.push_back({ .location = 1,
                                          .binding = 0,
                                          .format = VK_FORMAT_R32G32B32_SFLOAT,
                                          .offset = offsetof(vertex, Color) });
        attribute_description.push_back(
          { .location = 2,
            .binding = 0,
            .format = VK_FORMAT_R32G32B32_SFLOAT,
            .offset = offsetof(vertex, Normals) });
        attribute_description.push_back(
          { .location = 3,
            .binding = 0,
            .format = VK_FORMAT_R32G32_SFLOAT,
            .offset = offsetof(vertex, TexCoords) });
        return attribute_description;
    }

    void vk_shader::initialize_shader_module(const std::vector<char>& p_bin,
                                             VkShaderModule& p_shader_mod) {
        VkShaderModuleCreateInfo shader_mod_create_info = {
            .sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,
            .codeSize = p_bin.size(),
            .pCode = reinterpret_cast<const uint32_t*>(p_bin.data()),
        };

        vk_check(vkCreateShaderModule(vk_context::get_current_driver(),
                                      &shader_mod_create_info,
                                      nullptr,
                                      &p_shader_mod),
                 "vkCreateShaderModule",
                 __FILE__,
                 __LINE__,
                 __FUNCTION__);
    }

    shader_pipeline_config vk_shader::shader_configuration(uint32_t p_width,
                                                           uint32_t p_height) {
        shader_pipeline_config config{};

        // VkPipelineInputAssemblyStateCreateInfo pipeline_input_asm_create_info
        // = {
        config.PipelineInputAsmInfo = {
            .sType =
              VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO,
            .pNext = nullptr,
            .flags = 0,
            .topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST,
            .primitiveRestartEnable = false
        };

        config.Viewport = { .x = 0.0f,
                            .y = 0.0f,
                            .width = static_cast<float>(p_width),
                            .height = static_cast<float>(p_height),
                            .minDepth = 0.0f,
                            .maxDepth = 1.0f };

        config.Scissor = { .offset = { 0, 0 },
                           .extent = { p_width, p_height } };

        config.PipelineRasterizationCreateInfo = {
            .sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO,
            .pNext = nullptr,
            .flags = 0,
            .depthClampEnable = false,
            .rasterizerDiscardEnable = false,
            .polygonMode = VK_POLYGON_MODE_FILL,
            .cullMode = VK_CULL_MODE_NONE,
            .frontFace = VK_FRONT_FACE_CLOCKWISE,
            .depthBiasEnable = false,
            .depthBiasConstantFactor = 0.0f, // optional
            .depthBiasClamp = 0.0f,          // optional
            .depthBiasSlopeFactor = 0.0f,    // optional
            .lineWidth = 1.0f,
        };

        config.PipelineMultisampleCreateInfo = {
            .sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO,
            .pNext = nullptr,
            .flags = 0,
            .rasterizationSamples = VK_SAMPLE_COUNT_1_BIT,
            .sampleShadingEnable = false,
            .minSampleShading = 1.0f,       // optional
            .pSampleMask = nullptr,         // optional
            .alphaToCoverageEnable = false, // optional
            .alphaToOneEnable = false       // optional
        };

        config.PipelineColorBlendAttachments = {
            .blendEnable = false,
            .srcColorBlendFactor = VK_BLEND_FACTOR_ONE,  // optional
            .dstColorBlendFactor = VK_BLEND_FACTOR_ZERO, // optional
            .colorBlendOp = VK_BLEND_OP_ADD,             // optional
            .srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE,  // optional
            .dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO, // optional
            .alphaBlendOp = VK_BLEND_OP_ADD,             // optional
            .colorWriteMask =
              VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT |
              VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT
        };

        config.PipelineColorBlendCreateInfo = {
            .sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO,
            .pNext = nullptr,
            .flags = 0,
            .logicOpEnable = false,
            .logicOp = VK_LOGIC_OP_COPY,
            .attachmentCount = 1,
            .pAttachments = &config.PipelineColorBlendAttachments,
            .blendConstants = { 0.0f, 0.0f, 0.0f, 0.0f } // optional
            // .blendConstants = { // this is a float[4]
            //     {0.0f, 0.0f, 0.0f, 0.0f} // optional
            // }
        };

        config.PipelineDepthStencilCreateInfo = {
            .sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO,
            .pNext = nullptr,
            .flags = 0,
            .depthTestEnable = true,
            .depthWriteEnable = true,
            .depthCompareOp = VK_COMPARE_OP_LESS,
            .depthBoundsTestEnable = false,
            .stencilTestEnable = false,
            .front = {},
            .back = {},
            .minDepthBounds = 0.0f,
            .maxDepthBounds = 1.0f,
        };

        return config;
    }
};