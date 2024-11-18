#include "Editor.hpp"
#include "Core/TimeManagement/GlobalUpdateManager.hpp"
// #include "internal/VulkanCpp/helper_functions.hpp"
#include <Core/internal/Vulkan2Showcase/helper_functions.hpp>
#include <Core/internal/Vulkan2Showcase/VulkanContext.hpp>
#include <Core/Event/InputPoll.hpp>
#include <Core/Event/KeyCodes.hpp>
#include <Core/EngineLogger.hpp>
#include <Core/Timestep.hpp>
#include <glm/common.hpp>
#include <glm/gtc/constants.hpp>
#include <vulkan/vulkan_core.h>
// #include <Core/internal/Vulkan2Showcase/VulkanModel.hpp>
#include <array>
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

namespace engine3d{

    struct SimplePushConstantData{
        glm::mat2 Transform{1.f};
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

        // m_Shader = Shader::Create("simple_shader/simple_shader.vert.spv", "simple_shader/simple_shader.frag.spv", pipeline_config);
        m_Shader = Shader::Create("sim_shader_transforms/simple_shader.vert.spv", "sim_shader_transforms/simple_shader.frag.spv", pipeline_config);


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

        // triangle.SetColor({.1f, .8f, .1f});
        // triangle.SetColor({.1f, .8f, .1f});
        // triangle.SetColor({.1f, .8f, .1f});
        // Transform2DComponent transform;
        // transform.Translation.x = .2f;
        // transform.scale = {2.f, .5f};
        // transform.rotation = .25f * glm::two_pi<float>();
        // triangle.SetTranslation(transform);

        // m_GameObjects.push_back(std::move(triangle));
        std::vector<glm::vec3> colors{
            {1.f, .7f, .73f},
            {1.f, .87f, .73f},
            {1.f, 1.f, .73f},
            {.73f, 1.f, .8f},
            {.73, .88f, 1.f}  //
        };

        //! @note Loading 40 triangles.
        for(int i = 0; i < 40; i++){
            auto triangle = SceneObject::Create();
            triangle.SetModel(model);
            triangle.m_Transform2D.scale = glm::vec2(.5f) + i * 0.0025f;
            triangle.m_Transform2D.rotation = glm::pi<float>() * .00002f;
            triangle.SetColor(colors[i % colors.size()]);
            m_GameObjects.push_back(triangle);
        }

        GlobalUpdateManager::GetInstance()->SubscribeApplicationUpdate(this, &EditorApplication::OnApplicationUpdate);

    }

    EditorApplication::~EditorApplication() {}

    void EditorApplication::OnRender(uint32_t image_index){

        // for(uint32_t i = 0; i < m_CommandBuffers.size(); i++){
        VkCommandBufferBeginInfo cmd_buf_begin_info = {
            .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO
        };

        // START OF COMMAND BUFFER RECORD
        vk::vk_check(vkBeginCommandBuffer(m_CommandBuffers[image_index], &cmd_buf_begin_info), "vkBeginCommandBuffer", __FILE__, __LINE__, __FUNCTION__);

        // starting render pass
        VkRenderPassBeginInfo rp_begin_info = {
            .sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO,
            .renderPass = ApplicationInstance::GetWindow().GetCurrentSwapchain()->GetRenderPass(),
            .framebuffer = ApplicationInstance::GetWindow().GetCurrentSwapchain()->GetFramebuffer(image_index), // Specifying which framebuffer to render pass to.
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

        /*
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
        */

        vkCmdBeginRenderPass(m_CommandBuffers[image_index], &rp_begin_info, VK_SUBPASS_CONTENTS_INLINE);

        //! @note We can probably add this into Shader::Bind()
        //! @note Usage: m_Shader.Bind(m_CommandBuffers[i]);

        //! @note Give some cool effects
        int i = 0;
        for(auto& obj : m_GameObjects){
            i += 1;
            obj.m_Transform2D.rotation = glm::mod<float>(obj.m_Transform2D.rotation + 0.001f * i, 2.f * glm::pi<float>());
            // obj.m_Transform2D.Translation.x = glm::mod<float>(obj.m_Transform2D.rotation + 0.00001f * i, 2.f * glm::pi<float>());
            // obj.m_Transform2D.Translation.y = glm::mod<float>(obj.m_Transform2D.rotation + 0.00001f * i, 2.f * glm::pi<float>());

            // obj.SetRotation(glm::mod<float>(obj.m_Transform2D.rotation + 0.001f * i, 2.f * glm::pi<float>()));
        }


        //! @note Essentially doing m_Pipeline->Bind(m_CommandBuffer[i])
        //! @note Starts when to start rendering!!
        vkCmdBindPipeline(m_CommandBuffers[image_index], VK_PIPELINE_BIND_POINT_GRAPHICS, m_Shader->GetGraphicsPipeline());
        // vkCmdDraw(m_CommandBuffers[i], 3, 1, 0, 0);
        // model->Bind(m_CommandBuffers[i]);
        // model.Bind(m_CommandBuffers[i]);

        //! @note Only for testing purposes for mesh data.
        /*
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

            model->Draw(m_CommandBuffers[i]);
            // model.Draw(m_CommandBuffers[i]);
        }
        */

        //! @note 
        for(auto& obj : m_GameObjects){
            obj.m_Transform2D.rotation = glm::mod(obj.GetTransform().rotation + 0.001f, glm::two_pi<float>());

            SimplePushConstantData push = {
                .Transform = obj.GetTransform().mat2(),
                // .Transform = glm::mod(obj.GetTransform().rotation * 0.1f, glm::two_pi<float>()),
                .Offsets = obj.GetTransform().Translation,
                .Color = obj.GetColor(),
            };
            vkCmdPushConstants(
                m_CommandBuffers[image_index],
                m_PipelineLayout,
                VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT,
                0,
                    sizeof(SimplePushConstantData), 
                    &push
            );

            obj.GetModel()->Bind(m_CommandBuffers[image_index]);
            obj.GetModel()->Draw(m_CommandBuffers[image_index]);
        }

        vkCmdEndRenderPass(m_CommandBuffers[image_index]);

        // END OF COMMAND BUFFER RECORDING
        vk::vk_check(vkEndCommandBuffer(m_CommandBuffers[image_index]), "vkEndCommandBuffer", __FILE__, __LINE__, __FUNCTION__);
    }

    // void EditorApplication::UpdateThisApplicationInstance(){
    void EditorApplication::OnApplicationUpdate(){
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

        OnRender(image_index);

       ApplicationInstance::GetWindow().GetCurrentSwapchain()->SubmitCommandBuffer(&m_CommandBuffers[image_index]);
    }

    ApplicationInstance* InitializeApplication(){
        return new EditorApplication();
    }



};
