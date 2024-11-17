#include "Editor.hpp"
// #include "internal/VulkanCpp/helper_functions.hpp"
#include <Core/internal/Vulkan2Showcase/helper_functions.hpp>
#include <Core/internal/Vulkan2Showcase/VulkanContext.hpp>
#include <Core/Event/InputPoll.hpp>
#include <Core/Event/KeyCodes.hpp>
#include <Core/EngineLogger.hpp>
#include <Core/Timestep.hpp>
#include <vulkan/vulkan_core.h>
#include <Core/internal/Vulkan2Showcase/VulkanModel.hpp>

namespace engine3d{

    struct SimplePushConstantData{
        glm::vec2 Offsets;
        alignas(16) glm::vec3 Color;
    };

    EditorApplication::EditorApplication(const std::string& p_DebugName) : ApplicationInstance(p_DebugName) {
        // Renderer::Initialize();
        // Renderer::SetBackgroundColor({1.0f, 0.0f, 0.0f, 0.0f});
        
        // VulkanVertexBuffer vb = VulkanVertexBuffer({
        //     {{-0.5f, -0.288f, 0.0f, 1.0f}, {0.0f, 0.0f}},
        //     {{.5f, -.288f, 0.0f, 1.0f}, {1.0f, 0.0f}},
        //     {{0.0f, .577f, 0.0f, 1.0f}, {0.5f, 1.0f}}
        // });


        //! @note Basics of command buffers.

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

        vk::vk_check(vkCreatePipelineLayout(vk::VulkanContext::GetDriver(), &pipeline_layout_create_info, nullptr, &m_PipelineLayout), "vkCreatePipelineLayout", __FILE__, __LINE__, __FUNCTION__);
        
        //! @note Setting up our pipeline.
        auto pipeline_config = vk::VulkanShader::shader_configuration(ApplicationInstance::GetWindow().GetWidth(), ApplicationInstance::GetWindow().GetHeight());
        //! @note We are setting our shader pipeline to utilize our current window's swapchain
        //! @note a TODO is to utilize different render passes utiization for shader pipelines, potentially.
        pipeline_config.PipelineRenderPass = ApplicationInstance::GetWindow().GetCurrentSwapchain()->GetRenderPass();
        pipeline_config.PipelineLayout = m_PipelineLayout;

        m_Shader = vk::VulkanShader("simple_shader/simple_shader.vert.spv", "simple_shader/simple_shader.frag.spv", pipeline_config);


        //! @note Initializing Command buffers.
        m_CommandBuffers.resize(ApplicationInstance::GetWindow().GetCurrentSwapchain()->GetImagesSize());

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


        //! @note Loading triangle
        std::vector<vk::VulkanModel::Vertex> triangle_vertices = {
            {.Position={0.0f, -0.5f}, .Color = {1.0f, 0.0f, 0.0f}},
            {.Position ={0.5f, 0.5f}, .Color = {0.0f, 1.0f, 0.0f}},
            {.Position ={-0.5f, 0.5f}, .Color = {0.0f, 0.0f, 1.0f}}
        };

        vk::VulkanModel model = vk::VulkanModel(triangle_vertices, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);








        for(uint32_t i = 0; i < m_CommandBuffers.size(); i++){
            VkCommandBufferBeginInfo cmd_buf_begin_info = {
                .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO
            };

            // START OF COMMAND BUFFER RECORD
            vk::vk_check(vkBeginCommandBuffer(m_CommandBuffers[i], &cmd_buf_begin_info), "vkBeginCommandBuffer", __FILE__, __LINE__, __FUNCTION__);

            // starting render pass
            VkRenderPassBeginInfo rp_begin_info = {
                .sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO,
                .renderPass = ApplicationInstance::GetWindow().GetCurrentSwapchain()->GetRenderPass(),
                .framebuffer = ApplicationInstance::GetWindow().GetCurrentSwapchain()->GetFramebuffer(i), // Specifying which framebuffer to render pass to.
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

            if(ApplicationInstance::GetWindow().GetCurrentSwapchain()->GetRenderPass() == VK_NULL_HANDLE){
                ConsoleLogInfo("Renderpass Read at index  i = {} is not valid", i);
            }
            else{
                ConsoleLogInfo("Renderpass Read at index  i = {} is valid", i);
            }

            if(ApplicationInstance::GetWindow().GetCurrentSwapchain()->GetFramebuffer(i) == VK_NULL_HANDLE){
                ConsoleLogInfo("Framebuffer Read at index  i = {} is not valid", i);
            }
            else{
                ConsoleLogInfo("Framebuffer Read at index  i = {} is valid", i);
            }
            vkCmdBeginRenderPass(m_CommandBuffers[i], &rp_begin_info, VK_SUBPASS_CONTENTS_INLINE);

            //! @note We can probably add this into Shader::Bind()
            //! @note Usage: m_Shader.Bind(m_CommandBuffers[i]);
            vkCmdBindPipeline(m_CommandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, m_Shader.GetGraphicsPipeline());
            // vkCmdDraw(m_CommandBuffers[i], 3, 1, 0, 0);
            model.Bind(m_CommandBuffers[i]);

            for(int j = 0; j < 4; j++){
                SimplePushConstantData push{
                    .Offsets = {0.0f, -0.4f + j * 0.25f},
                    .Color = {0.0f, 0.0f, 0.2f + 0.2f * j}
                };

                vkCmdPushConstants(
                    m_CommandBuffers[i],
                     m_PipelineLayout,
                      VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT,
                       0,
                        sizeof(SimplePushConstantData), 
                        &push
                );

                model.Draw(m_CommandBuffers[i]);
            }

            vkCmdEndRenderPass(m_CommandBuffers[i]);

            // END OF COMMAND BUFFER RECORDING
            vk::vk_check(vkEndCommandBuffer(m_CommandBuffers[i]), "vkEndCommandBuffer", __FILE__, __LINE__, __FUNCTION__);
            ConsoleLogInfo("Just after Command Buffer Recording");
        }
    }

    EditorApplication::~EditorApplication() {}

    void EditorApplication::UpdateThisApplicationInstance(){
        
        //! @note Just testing to see if application still closes cleanly.
        // if(InputPoll::IsKeyPressed(ENGINE_KEY_ESCAPE)){
        //     exit(0);
        // }

        //! @note This function will render our primitives
        //! @note TODO --  Flush should only happens when our scene is given everything that lives within this scene (ref to lifetimes)
        /* Renderer::FlushScene(); */
		
		// Renderer::Presentation();
        // ImGui::Begin("Hello, world!"); // Create a window called "Hello, world!" and append into it.
        // ImGui::End();

        /*
        @note This is going to be what we will implement next in the Renderer.hpp/.cpp once we can render the triangle.
        @note The idea is in the application this the swapchain should not be called directly by the application developer.
        @note Want to migrate to doing this so its easier for testability and submitting commands/drawcalls to vulkan's API.
        @note Realistically we'll have VulkanRenderer that would be accepting these structures.
        Renderer::Submit([](){
            vkCmdBeginRenderPass(m_CommandBuffers[i], &rp_begin_info, VK_SUBPASS_CONTENTS_INLINE);

            vkCmdBindPipeline(m_CommandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, m_Shader.GetGraphicsPipeline());
            vkCmdDraw(m_CommandBuffers[i], 3, 1, 0, 0);

            vkCmdEndRenderPass(m_CommandBuffers[i]);

            vk::vk_check(vkEndCommandBuffer(m_CommandBuffers[i]), "vkEndCommandBuffer", __FILE__, __LINE__, __FUNCTION__);
        });
        */
        uint32_t image_index = ApplicationInstance::GetWindow().GetCurrentSwapchain()->AcquireNextImage();

        for(int j = 0; j < 4; j++){
                SimplePushConstantData push{
                    .Offsets = {0.0f, -0.4f + j * 0.25f},
                    .Color = {0.0f, 0.0f, 0.2f + 0.2f * j + 1}
                };

                vkCmdPushConstants(
                    m_CommandBuffers[image_index],
                     m_PipelineLayout,
                      VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT,
                       0,
                        sizeof(SimplePushConstantData), 
                        &push
                );
            }

       ApplicationInstance::GetWindow().GetCurrentSwapchain()->SubmitCommandBuffer(&m_CommandBuffers[image_index]);
    }

    ApplicationInstance* InitializeApplication(){
        return new EditorApplication();
    }



};
