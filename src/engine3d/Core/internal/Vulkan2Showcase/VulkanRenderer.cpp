#include "EngineLogger.hpp"
#include "internal/Vulkan2Showcase/VulkanContext.hpp"
#include <Core/internal/Vulkan2Showcase/Shaders/VulkanShader.hpp>
#include <Core/ApplicationInstance.hpp>
#include <internal/Vulkan2Showcase/VulkanRenderer.hpp>
#include <internal/Vulkan2Showcase/helper_functions.hpp>
#include <array>

namespace engine3d::vk{
    struct SimplePushConstantData{
        glm::mat2 Transform{1.f};
        glm::vec2 Offsets;
        alignas(16) glm::vec3 Color;
    };

    VulkanRenderer::VulkanRenderer(const std::string& p_DebugName){
        ConsoleLogError("VulkanRenderer Initializing....");
        m_Swapchain = ApplicationInstance::GetWindow().GetCurrentSwapchain();
        //! @note Setting up our pipeline.
        auto pipeline_config = vk::VulkanShader::shader_configuration(ApplicationInstance::GetWindow().GetWidth(), ApplicationInstance::GetWindow().GetHeight());
        // //! @note We are setting our shader pipeline to utilize our current window's swapchain
        // //! @note a TODO is to utilize different render passes utiization for shader pipelines, potentially.
        pipeline_config.PipelineRenderPass = m_Swapchain->GetRenderPass();
        pipeline_config.PipelineLayout = m_PipelineLayout;

        // ConsoleLogWarn("Just before loading shaders!!");
        m_Shader = Shader::Create("sim_shader_transforms/simple_shader.vert.spv", "sim_shader_transforms/simple_shader.frag.spv", pipeline_config);

        // ConsoleLogWarn("Initializing vulkan renderer2!");
        // //! @note Initializing Command buffers.
        m_CommandBuffers.resize(ApplicationInstance::GetWindow().GetCurrentSwapchain()->GetImagesSize());

        // m_CommandBuffers.resize(ApplicationInstance::GetWindow().GetCurrentSwapchain()->GetImagesSize());

        //! @note Creating our pools of command buffer structs
        VkCommandPoolCreateInfo pool_create_info = {
            .sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
            .pNext = nullptr,
            .flags = VK_COMMAND_POOL_CREATE_TRANSIENT_BIT | VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT,
            .queueFamilyIndex = vk::VulkanContext::GetPhysicalDriver().GetQueueIndices().Graphics
        };

        vk::vk_check(vkCreateCommandPool(vk::VulkanContext::GetDriver(), &pool_create_info, nullptr, &m_CommandPool), "vkCreateCommandPool", __FILE__, __LINE__, __FUNCTION__);

        //! @note Allocating our command buffers.
        VkCommandBufferAllocateInfo cmd_buffer_alloc_info = {
            .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
            .commandPool = m_CommandPool,
            .level = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
            .commandBufferCount = static_cast<uint32_t>(m_CommandBuffers.size()),
        };

        vk::vk_check(vkAllocateCommandBuffers(vk::VulkanContext::GetDriver(), &cmd_buffer_alloc_info, m_CommandBuffers.data()), "vkAllocateCommandBuffers", __FILE__, __LINE__, __FUNCTION__);

        ConsoleLogInfo("CommandBuffers Size === {}", m_CommandBuffers.size());

        // ConsoleLogInfo("CommandBuffers Size === {}", m_CommandBuffers.size());

        // VkPushConstantRange push_const_range = {
        //     .stageFlags =  VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT,
        //     .offset = 0,
        //     .size = sizeof(SimplePushConstantData)
        // };

        // //! @note First initializing pipeline layout create info
        // VkPipelineLayoutCreateInfo pipeline_layout_create_info = {
        //     .sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
        //     .pNext = nullptr,
        //     .flags = 0,
        //     .setLayoutCount = 0,
        //     .pSetLayouts = nullptr,
        //     // .pushConstantRangeCount = 0,
        //     // .pPushConstantRanges = nullptr,
        //     .pushConstantRangeCount = 1,
        //     .pPushConstantRanges = &push_const_range
        // };

        // vk::vk_check(vkCreatePipelineLayout(vk::VulkanContext::GetDriver(), &pipeline_layout_create_info, nullptr, &m_PipelineLayout), "vkCreatePipelineLayout", __FILE__, __LINE__, __FUNCTION__);
        
    }

    void VulkanRenderer::StartFrame(){
        ConsoleLogInfo("VulkanRenderer::BeginFrame");
        m_CurrentFrameIndex = m_Swapchain->AcquireNextImage();
        ConsoleLogError("NOT AN ERROR JUST DEBUGGING: Current Frame Index === {}", m_CurrentFrameIndex);
        auto current_cmd_buffer = CurrentCommandBuffer();

    }

    //! @note When we reach the end of the frame that is when we start rendering.
    //! @note Beginning of frame we record, and write.
    //! @note End of frame we render and submit rendering commands.
    void VulkanRenderer::FinishFrame(){
        
        ConsoleLogInfo("VulkanRenderer::EndFrame()");
        auto current_cmd_buffer = CurrentCommandBuffer();

        VkCommandBufferBeginInfo cmd_buf_begin_info = {
            .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO
        };

        // START OF COMMAND BUFFER RECORD
        vk::vk_check(vkBeginCommandBuffer(current_cmd_buffer, &cmd_buf_begin_info), "vkBeginCommandBuffer", __FILE__, __LINE__, __FUNCTION__);
        
        // starting render pass
        VkRenderPassBeginInfo rp_begin_info = {
            .sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO,
            .renderPass = ApplicationInstance::GetWindow().GetCurrentSwapchain()->GetRenderPass(),
            .framebuffer = ApplicationInstance::GetWindow().GetCurrentSwapchain()->GetFramebuffer(m_CurrentFrameIndex), // Specifying which framebuffer to render pass to.
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

        vkCmdBeginRenderPass(current_cmd_buffer, &rp_begin_info, VK_SUBPASS_CONTENTS_INLINE);

        //--------------------------------------
        //! @note Rendering stuff....
        //--------------------------------------
        // int i = 0;
        // for(auto& obj : m_GameObjects){
        //     i += 1;
        //     obj.m_Transform2D.rotation = glm::mod<float>(obj.m_Transform2D.rotation + 0.001f * i, 2.f * glm::pi<float>());
        // }


        //! @note Essentially doing m_Pipeline->Bind(m_CommandBuffer[i])
        //! @note Starts when to start rendering!!
        // vkCmdBindPipeline(current_cmd_buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_Shader->GetGraphicsPipeline());

        // for(auto& obj : m_GameObjects){
        //     obj.m_Transform2D.rotation = glm::mod(obj.GetTransform().rotation + 0.001f, glm::two_pi<float>());

        //     SimplePushConstantData push = {
        //         .Transform = obj.GetTransform().mat2(),
        //         // .Transform = glm::mod(obj.GetTransform().rotation * 0.1f, glm::two_pi<float>()),
        //         .Offsets = obj.GetTransform().Translation,
        //         .Color = obj.GetColor(),
        //     };
        //     vkCmdPushConstants(
        //         current_cmd_buffer,
        //         m_PipelineLayout,
        //         VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT,
        //         0,
        //             sizeof(SimplePushConstantData), 
        //             &push
        //     );

        //     obj.GetModel()->Bind(current_cmd_buffer);
        //     obj.GetModel()->Draw(current_cmd_buffer);
        // }


        //--------------------------------------
        //! @note Rendering stuff....
        //--------------------------------------


        vkCmdEndRenderPass(current_cmd_buffer);

        // END OF COMMAND BUFFER RECORDING
        vk::vk_check(vkEndCommandBuffer(current_cmd_buffer), "vkEndCommandBuffer", __FILE__, __LINE__, __FUNCTION__);

        ApplicationInstance::GetWindow().GetCurrentSwapchain()->SubmitCommandBuffer(&current_cmd_buffer);
    }


    VkCommandBuffer VulkanRenderer::CurrentCommandBuffer(){
        return m_CommandBuffers[m_CurrentFrameIndex];
    }
};