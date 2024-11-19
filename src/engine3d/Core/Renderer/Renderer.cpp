// #include "internal/VulkanCpp/Vulkan.hpp"
#include "EngineLogger.hpp"
#include "GraphicDrivers/Shader.hpp"
#include "internal/Vulkan2Showcase/Shaders/VulkanShader.hpp"
#include "internal/Vulkan2Showcase/VulkanContext.hpp"
#include "internal/Vulkan2Showcase/VulkanModel.hpp"
#include "internal/Vulkan2Showcase/helper_functions.hpp"
#include <Core/ApplicationInstance.hpp>
#include <Core/Renderer/Renderer.hpp>
#include <vulkan/vulkan_core.h>
#include <vector>

#include <array>
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

namespace engine3d{
    static std::vector<VkCommandBuffer> g_CommandBuffers;
    VkCommandPool g_CmdPool;
    uint32_t g_CurrentFrameIndex = -1;
    static bool g_IsFrameStarted = false;

    static Ref<Shader> g_Shader = nullptr;
    static VkPipelineLayout g_PipelineLayout;
    static VkPipeline g_Pipeline;

    // static std::vector<SceneObject> g_GameObjects;

    struct SimplePushConstantData{
        glm::mat2 Transform{1.f};
        glm::vec2 Offsets;
        glm::vec2 iResolution;
        alignas(16) glm::vec3 Color;
    };

    void Renderer::Initialize(const std::string& p_DebugName){
        ConsoleLogInfo("RENDERER BEGIN INTIALIZATION!!!");
        g_CurrentFrameIndex = 0;
        g_IsFrameStarted = false;

        //! @note Setting up our pipeline.
        auto pipeline_config = vk::VulkanShader::shader_configuration(ApplicationInstance::GetWindow().GetWidth(), ApplicationInstance::GetWindow().GetHeight());

        //! @note Initialize Push constant range
        VkPushConstantRange push_const_range = {
            .stageFlags =  VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT,
            .offset = 0,
            .size = sizeof(SimplePushConstantData)
        };

        //! @note First initializing pipeline layout create info
        VkPipelineLayoutCreateInfo pipeline_layout_create_info = {
            .sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
            .pNext = nullptr,
            .flags = 0,
            .setLayoutCount = 0,
            .pSetLayouts = nullptr,
            // .pushConstantRangeCount = 0,
            // .pPushConstantRanges = nullptr,
            .pushConstantRangeCount = 1,
            .pPushConstantRanges = &push_const_range
        };

        vk::vk_check(vkCreatePipelineLayout(vk::VulkanContext::GetDriver(), &pipeline_layout_create_info, nullptr, &g_PipelineLayout), "vkCreatePipelineLayout", __FILE__, __LINE__, __FUNCTION__);
        

        //! @note We are setting our shader pipeline to utilize our current window's swapchain
        //! @note a TODO is to utilize different render passes utiization for shader pipelines, potentially.
        pipeline_config.PipelineRenderPass = ApplicationInstance::GetWindow().GetCurrentSwapchain()->GetRenderPass();
        pipeline_config.PipelineLayout = g_PipelineLayout;

        // m_Shader = Shader::Create("simple_shader/simple_shader.vert.spv", "simple_shader/simple_shader.frag.spv", pipeline_config);
        g_Shader = Shader::Create("sim_shader_transforms/simple_shader.vert.spv", "sim_shader_transforms/simple_shader.frag.spv", pipeline_config);

        ConsoleLogError("NOT AN ERROR: Shader Loaded Successfully!");





        //! @note Initializing Command buffers.
        g_CommandBuffers.resize(ApplicationInstance::GetWindow().GetCurrentSwapchain()->GetImagesSize());

        VkCommandPoolCreateInfo pool_create_info = {
            .sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
            .pNext = nullptr,
            .flags = VK_COMMAND_POOL_CREATE_TRANSIENT_BIT | VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT,
            .queueFamilyIndex = vk::VulkanContext::GetPhysicalDriver().GetQueueIndices().Graphics
        };

        vk::vk_check(vkCreateCommandPool(vk::VulkanContext::GetDriver(), &pool_create_info, nullptr, &g_CmdPool), "vkCreateCommandPool", __FILE__, __LINE__, __FUNCTION__);


        ConsoleLogInfo("RENDERER COMMAND BUFFERS SIZE === {}", g_CommandBuffers.size());
        //! @note Allocating our command buffers.
        VkCommandBufferAllocateInfo cmd_buffer_alloc_info = {
            .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
            .commandPool = g_CmdPool,
            .level = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
            .commandBufferCount = static_cast<uint32_t>(g_CommandBuffers.size()),
        };

        vk::vk_check(vkAllocateCommandBuffers(vk::VulkanContext::GetDriver(), &cmd_buffer_alloc_info, g_CommandBuffers.data()), "vkAllocateCommandBuffers", __FILE__, __LINE__, __FUNCTION__);

        ConsoleLogInfo("CommandBuffers Size === {}", g_CommandBuffers.size());

        //--------------------------------------------
        //! @note Loading Game Objectts Here
        //--------------------------------------------
        std::vector<vk::VulkanModel::Vertex> triangle_vertices = {
            {.Position={0.0f, -0.5f}, .Color = {1.0f, 0.0f, 0.0f}},
            {.Position ={0.5f, 0.5f}, .Color = {0.0f, 1.0f, 0.0f}},
            {.Position ={-0.5f, 0.5f}, .Color = {0.0f, 0.0f, 1.0f}}
        };
        // vk::VulkanModel model = vk::VulkanModel(triangle_vertices, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
        auto model = CreateRef<vk::VulkanModel>(triangle_vertices, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
        auto triangle = SceneObject::Create();
        triangle.SetModel(model);

        //! @note Push Constants is pretty much how we can modify data without needing to record command buffers again.

        // g_GameObjects.push_back(std::move(triangle));
        std::vector<glm::vec3> colors{
            {1.f, .7f, .73f},
            {1.f, .87f, .73f},
            {1.f, 1.f, .73f},
            {.73f, 1.f, .8f},
            {.73, .88f, 1.f}  //
        };

        //! @note Loading 40 triangles.
        // for(int i = 0; i < 40; i++){
        //     auto triangle = SceneObject::Create();
        //     triangle.SetModel(model);
        //     triangle.m_Transform2D.scale = glm::vec2(.5f) + i * 0.0025f;
        //     triangle.m_Transform2D.rotation = glm::pi<float>() * .00002f;
        //     triangle.SetColor(colors[i % colors.size()]);
        //     g_GameObjects.push_back(triangle);
        // }

    }

    void Renderer::SetBackgroundColor(const std::array<float, 4>& p_Rgba){
        VkClearValue clearColorValue = {{p_Rgba[0], p_Rgba[1], p_Rgba[2], p_Rgba[3]}};
    }

    VkCommandBuffer Renderer::BeginFrame(){
        g_CurrentFrameIndex = ApplicationInstance::GetWindow().GetCurrentSwapchain()->AcquireNextImage();
        VkCommandBufferBeginInfo cmd_buffer_begin_info = {
            .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO
        };

        auto cmd_buffer = GetCurrentCommandBuffer();

        vk::vk_check(vkBeginCommandBuffer(cmd_buffer, &cmd_buffer_begin_info), "vkBeginCommandBuffer", __FILE__, __LINE__, __FUNCTION__);
        VkRenderPassBeginInfo rp_begin_info = {
            .sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO,
            .renderPass = ApplicationInstance::GetWindow().GetCurrentSwapchain()->GetRenderPass(),
            .framebuffer = ApplicationInstance::GetWindow().GetCurrentSwapchain()->GetFramebuffer(g_CurrentFrameIndex), // Specifying which framebuffer to render pass to.
            .renderArea = {
                .offset = {0, 0},
                .extent = {ApplicationInstance::GetWindow().GetWidth(), ApplicationInstance::GetWindow().GetHeight()}
            }
        };

        std::array<VkClearValue, 2> clearValues;
        clearValues[0].color = {0.1f, 0.1f, 0.1f, 1.0f};
        clearValues[1].depthStencil = {1.0f, 0};

        rp_begin_info.clearValueCount = static_cast<uint32_t>(clearValues.size());
        rp_begin_info.pClearValues = clearValues.data();


        vkCmdBeginRenderPass(cmd_buffer, &rp_begin_info, VK_SUBPASS_CONTENTS_INLINE);
        g_IsFrameStarted = true;
        return cmd_buffer;
    }

    void Renderer::RecordCommandBuffers(VkCommandBuffer p_CommandBuffer){
        //! @note We can probably add this into Shader::Bind()
        //! @note Usage: m_Shader.Bind(m_CommandBuffers[i]);
        /*
        int i = 0;
        for(auto& obj : g_GameObjects){
            i += 1;
            obj.m_Transform2D.rotation = glm::mod<float>(obj.m_Transform2D.rotation + 0.001f * i, 2.f * glm::pi<float>());
        }


        //! @note Essentially doing m_Pipeline->Bind(m_CommandBuffer[i])
        //! @note Starts when to start rendering!!
        vkCmdBindPipeline(p_CommandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, g_Shader->GetGraphicsPipeline());

        //! @note Only for testing purposes for mesh data.
        for(auto& obj : g_GameObjects){
            obj.m_Transform2D.rotation = glm::mod(obj.GetTransform().rotation + 0.001f, glm::two_pi<float>());

            SimplePushConstantData push = {
                .Transform = obj.GetTransform().mat2(),
                // .Transform = glm::mod(obj.GetTransform().rotation * 0.1f, glm::two_pi<float>()),
                .Offsets = obj.GetTransform().Translation,
                .Color = obj.GetColor(),
            };
            vkCmdPushConstants(
                p_CommandBuffer,
                g_PipelineLayout,
                VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT,
                0,
                    sizeof(SimplePushConstantData), 
                    &push
            );

            obj.GetModel()->Bind(p_CommandBuffer);
            obj.GetModel()->Draw(p_CommandBuffer);
        }

        */
    }

    void Renderer::RecordGameObjects(std::vector<SceneObject>& p_Objects){
        auto current_cmd_buffer = GetCurrentCommandBuffer();
        //! @note Essentially doing m_Pipeline->Bind(m_CommandBuffer[i])
        //! @note Starts when to start rendering!!
        vkCmdBindPipeline(current_cmd_buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, g_Shader->GetGraphicsPipeline());

        //! @note Only for testing purposes for mesh data.
        for(auto& obj : p_Objects){
            // obj.m_Transform2D.rotation = glm::mod(obj.GetTransform().rotation + 0.001f, glm::two_pi<float>());

            SimplePushConstantData push = {
                .Transform = obj.GetTransform().mat2(),
                // .Transform = glm::mod(obj.GetTransform().rotation * 0.1f, glm::two_pi<float>()),
                .Offsets = obj.GetTransform().Translation,
                .iResolution = {ApplicationInstance::GetWindow().GetWidth(), ApplicationInstance::GetWindow().GetHeight()},
                .Color = obj.GetColor(),
            };
            vkCmdPushConstants(
                current_cmd_buffer,
                g_PipelineLayout,
                VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT,
                0,
                    sizeof(SimplePushConstantData), 
                    &push
            );

            obj.GetModel()->Bind(current_cmd_buffer);
            obj.GetModel()->Draw(current_cmd_buffer);
        }
    }

    void Renderer::EndFrame(){
        auto cmd_buffer = GetCurrentCommandBuffer();
        vkCmdEndRenderPass(cmd_buffer);
        vk::vk_check(vkEndCommandBuffer(cmd_buffer), "vkEndCommandBuffer", __FILE__, __LINE__, __FUNCTION__);

        ApplicationInstance::GetWindow().GetCurrentSwapchain()->SubmitCommandBuffer(&cmd_buffer);
        g_IsFrameStarted = false;
    }

    VkCommandBuffer Renderer::GetCurrentCommandBuffer(){
        return g_CommandBuffers[g_CurrentFrameIndex];
    }
};