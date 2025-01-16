#include <drivers/vulkan/shaders/vulkan_shader.hpp>
#include <core/engine_logger.hpp>
#include <drivers/vulkan/helper_functions.hpp>
#include <drivers/vulkan/vulkan_context.hpp>
#include <fstream>
#include <drivers/vertex_buffer.hpp>

namespace engine3d::vk{
    VulkanShader::VulkanShader(const std::string& p_VertShader, const std::string& p_FragShader, const ShaderPipelineConfig& p_Config){
        initialize_graphics_pipeline(p_VertShader, p_FragShader, p_Config);
    }

    std::vector<char> VulkanShader::read_file(const std::string& p_Filepath){
        std::ifstream ins(p_Filepath, std::ios::ate | std::ios::binary);

        if(!ins){
            ConsoleLogError("Could not read in file \"{}\"", p_Filepath);
            return {};
        }
        size_t file_size = static_cast<size_t>(ins.tellg());
        std::vector<char> buffer(file_size);

        ins.seekg(0);
        ins.read(buffer.data(), file_size);
        return buffer;
    }

    void VulkanShader::initialize_graphics_pipeline(const std::string& p_VertShader, const std::string& p_FragShader, const ShaderPipelineConfig& p_Config){
        auto vert = read_file(p_VertShader);
        auto frag = read_file(p_FragShader);

        initialize_shader_module(vert, m_VertexShaderModule);
        initialize_shader_module(frag, m_FragmentShaderModule);

        //! @note Setting vertex/fragment shaders stages.

        std::array<VkPipelineShaderStageCreateInfo, 2> shader_stages;

        VkPipelineShaderStageCreateInfo vert_shader_stage = {
            .sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
            .pNext = nullptr,
            .flags = 0,
            .stage = VK_SHADER_STAGE_VERTEX_BIT,
            .module = m_VertexShaderModule,
            .pName = "main"
        };

        VkPipelineShaderStageCreateInfo frag_shader_stage = {
            .sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
            .pNext = nullptr,
            .flags = 0,
            .stage = VK_SHADER_STAGE_FRAGMENT_BIT,
            .module = m_FragmentShaderModule,
            .pName = "main"
        };

        shader_stages[0] = vert_shader_stage;
        shader_stages[1] = frag_shader_stage;

        auto binding_description = GetVertexInputBindDescription();
        auto attachment_description = GetVertexAttributeDescriptions();

        VkPipelineVertexInputStateCreateInfo vert_input_create_info = {
            .sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO,
            .pNext = nullptr,
            .vertexBindingDescriptionCount = static_cast<uint32_t>(binding_description.size()),
            .pVertexBindingDescriptions = binding_description.data(),
            .vertexAttributeDescriptionCount = static_cast<uint32_t>(attachment_description.size()),
            .pVertexAttributeDescriptions = attachment_description.data(),
        };

        VkPipelineViewportStateCreateInfo PipelineViewportCreateInfo = {
            .sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO,
            .pNext = nullptr,
            .flags = 0,
            .viewportCount = 1,
            .pViewports = &p_Config.Viewport,
            .scissorCount = 1,
            .pScissors = &p_Config.Scissor
        };

        VkGraphicsPipelineCreateInfo graphics_pipeline_create_info = {
            .sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO,
            .stageCount = 2,
            .pStages = shader_stages.data(),
            .pVertexInputState = &vert_input_create_info,
            .pInputAssemblyState = &p_Config.PipelineInputAsmInfo,
            .pViewportState = &PipelineViewportCreateInfo,
            .pRasterizationState = &p_Config.PipelineRasterizationCreateInfo,
            .pMultisampleState = &p_Config.PipelineMultisampleCreateInfo,
            .pDepthStencilState = &p_Config.PipelineDepthStencilCreateInfo,
            .pColorBlendState = &p_Config.PipelineColorBlendCreateInfo,
            .pDynamicState = nullptr,
            .layout = p_Config.PipelineLayout,
            .renderPass = p_Config.PipelineRenderPass,
            .subpass = p_Config.SubpassCount,
            .basePipelineHandle = nullptr,
            .basePipelineIndex = -1,
        };

        vk_check(vkCreateGraphicsPipelines(VulkanContext::GetDriver(), nullptr, 1, &graphics_pipeline_create_info, nullptr, &m_GraphicsPipeline), "vkCreateGraphicsPipelines", __FILE__, __LINE__, __FUNCTION__);

        ConsoleLogWarn("Vertex Shader Size == {}", vert.size());
        ConsoleLogWarn("Fragment Shader Size == {}", frag.size());
    }

    std::vector<VkVertexInputBindingDescription> VulkanShader::GetVertexInputBindDescription(){
        std::vector<VkVertexInputBindingDescription> binding_descriptions(1);
        binding_descriptions[0] = {
            .binding = 0,
            .stride = sizeof(Vertex),
            .inputRate = VK_VERTEX_INPUT_RATE_VERTEX
        };

        return binding_descriptions;
    }

    std::vector<VkVertexInputAttributeDescription> VulkanShader::GetVertexAttributeDescriptions(){
        std::vector<VkVertexInputAttributeDescription> attribute_description{};

        attribute_description.push_back({0, 0, VK_FORMAT_R32G32B32_SFLOAT, offsetof(Vertex, Position)});
        attribute_description.push_back({1, 0, VK_FORMAT_R32G32B32_SFLOAT, offsetof(Vertex, Color)});
        attribute_description.push_back({2, 0, VK_FORMAT_R32G32B32_SFLOAT, offsetof(Vertex, Normals)});
        attribute_description.push_back({3, 0, VK_FORMAT_R32G32_SFLOAT, offsetof(Vertex, Uv)});
        return attribute_description;
    }

    void VulkanShader::initialize_shader_module(const std::vector<char>& p_Bin, VkShaderModule& p_ShaderMod){
        VkShaderModuleCreateInfo shader_mod_create_info = {
            .sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,
            .codeSize = p_Bin.size(),
            .pCode = reinterpret_cast<const uint32_t*>(p_Bin.data())
        };

        vk_check(vkCreateShaderModule(VulkanContext::GetDriver(), &shader_mod_create_info, nullptr, &p_ShaderMod), "vkCreateShaderModule", __FILE__, __LINE__, __FUNCTION__);

    }

    ShaderPipelineConfig VulkanShader::shader_configuration(uint32_t p_Width, uint32_t p_Height){
        ShaderPipelineConfig config{};

        // VkPipelineInputAssemblyStateCreateInfo pipeline_input_asm_create_info = {
        config.PipelineInputAsmInfo = {
            .sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO,
            .pNext = nullptr,
            .flags = 0,
            .topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST,
            .primitiveRestartEnable = false
        };

        config.Viewport = {
            .x = 0.0f,
            .y = 0.0f,
            .width = static_cast<float>(p_Width),
            .height = static_cast<float>(p_Height),
            .minDepth = 0.0f,
            .maxDepth = 1.0f
        };

        config.Scissor = {
            .offset = {0, 0},
            .extent = {p_Width, p_Height}
        };
      
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
            .srcColorBlendFactor = VK_BLEND_FACTOR_ONE,     // optional
            .dstColorBlendFactor = VK_BLEND_FACTOR_ZERO,    // optional
            .colorBlendOp = VK_BLEND_OP_ADD,                // optional
            .srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE,     // optional
            .dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO,    // optional
            .alphaBlendOp = VK_BLEND_OP_ADD,                // optional
            .colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT
        };

        config.PipelineColorBlendCreateInfo = {
            .sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO,
            .pNext = nullptr,
            .flags = 0,
            .logicOpEnable = false,
            .logicOp = VK_LOGIC_OP_COPY,
            .attachmentCount = 1,
            .pAttachments = &config.PipelineColorBlendAttachments,
            .blendConstants = {0.0f, 0.0f, 0.0f, 0.0f} // optional
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