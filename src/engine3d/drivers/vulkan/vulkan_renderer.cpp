
#include "scene/scene.hpp"
#include "update_handlers/sync_update.hpp"
#include <deque>
#include <numeric>
#include <drivers/vulkan/vulkan_renderer.hpp>
#include <vector>
#include <vulkan/vulkan_core.h>
#include <atomic>
#include <drivers/vulkan/vulkan_context.hpp>
#include <drivers/vulkan/helper_functions.hpp>
#include <core/application_instance.hpp>
#include <core/engine_logger.hpp>
#include <drivers/vulkan/vulkan_swapchain.hpp>
#include <drivers/vulkan/shaders/vulkan_shader.hpp>
#include <scene/scene_object.hpp>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

namespace engine3d::vk{
    static std::vector<VkCommandBuffer> g_CommandBuffers;
    VkCommandPool g_CommandPool;
    std::atomic<uint32_t> g_CurrentFrameIndex = -1;
    static bool g_IsFrameStarted = false;
    static VkPipelineLayout g_PipelineLayout;
    // static VkPipeline g_Pipeline;

    static std::deque<std::function<void()>> s_MainDeletionQueue;

    template<typename UFunction>
    void SubmitResourceFree(const UFunction& p_TaskToDelete){
        s_MainDeletionQueue.push_back(p_TaskToDelete);
    }

    //! @note Shader-definitions
    static Ref<Shader> g_Shader = nullptr;

    struct PushConstantData{
        // glm::mat4 Transform{1.f};
        glm::mat4 Projection{1.f};
        glm::mat4 View{1.f};
        glm::mat4 Model{1.f};
        glm::vec3 LightTransform{1.0, -3.0, -1.0};
    };

    void VulkanRenderer::InitializeRendererPipeline(){
        //! @note Setting up push constants
        //! @note VkPipelineLayoutCreateInfo does not require a push constant to be defined in it's configuration
        //! @note It can just be set to its default vulkan has its configuration
        g_CurrentFrameIndex = 0;
        g_IsFrameStarted = false;

        //! @note Setting up our pipeline.
        auto pipeline_config = vk::VulkanShader::shader_configuration(ApplicationInstance::GetWindow().GetWidth(), ApplicationInstance::GetWindow().GetHeight());

        //! @note Initialize Push constant range
        VkPushConstantRange push_const_range = {
            .stageFlags =  VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT,
            .offset = 0,
            .size = sizeof(PushConstantData)
        };

        //! @note First initializing pipeline layout create info
        VkPipelineLayoutCreateInfo pipeline_layout_create_info = {
            .sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
            .pNext = nullptr,
            .flags = 0,
            .setLayoutCount = 0,
            .pSetLayouts = nullptr,
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

        vk::vk_check(vkCreateCommandPool(vk::VulkanContext::GetDriver(), &pool_create_info, nullptr, &g_CommandPool), "vkCreateCommandPool", __FILE__, __LINE__, __FUNCTION__);


        ConsoleLogInfo("RENDERER COMMAND BUFFERS SIZE === {}", g_CommandBuffers.size());
        //! @note Allocating our command buffers.
        VkCommandBufferAllocateInfo cmd_buffer_alloc_info = {
            .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
            .commandPool = g_CommandPool,
            .level = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
            .commandBufferCount = static_cast<uint32_t>(g_CommandBuffers.size()),
        };

        vk::vk_check(vkAllocateCommandBuffers(vk::VulkanContext::GetDriver(), &cmd_buffer_alloc_info, g_CommandBuffers.data()), "vkAllocateCommandBuffers", __FILE__, __LINE__, __FUNCTION__);

        ConsoleLogInfo("CommandBuffers Size === {}", g_CommandBuffers.size());
    }

    VulkanRenderer::VulkanRenderer(const std::string& p_Tag){
        g_CurrentFrameIndex = 0;
        g_IsFrameStarted = false;
        // ConsoleEngineLogger::AddNewLogger(p_Tag);
        if(p_Tag.empty()){}
        InitializeRendererPipeline();
    }

    void VulkanRenderer::BeginFrame(){
        //! @note Note to Self: This is just a hack on how to get "resizing" to work
        //! @note I will need to do this better as there are problems with this.
        //! @note Currently the state is if the window is resized by glfwSetFramebufferSizeCallback in vulkan_window.cpp, we also check here for the state of our swapchain.
        //! @note If our swapchain's state is either VK_ERROR_OUT_OF_DATE_KHR or VK_SUBOPTIMAL_KHR, then the Acquired function returns -3.
        //! @note -3 was just to indicate the swapchain's state as been modified and needs to modify the pipeline to work with the new swapchain.
        //! @note Another TODO: Make abstract the pipeline's so that we do not have to modify this directly.
        //! @note If this isn't already done, but the vk::submit function should handle the way we directly handle these tasks... potentially?
        g_CurrentFrameIndex = ApplicationInstance::GetWindow().GetCurrentSwapchain()->AcquireNextImage();
        // ConsoleLogInfoWithTag("vulkan", "Current Frame Index = {}", g_CurrentFrameIndex.load());
        // ConsoleLogWarnWithTag("vulkan", "IsResized = {}", ApplicationInstance::GetWindow().GetCurrentSwapchain()->IsResized());
        // if(ApplicationInstance::GetWindow().GetCurrentSwapchain()->IsResized()){
        // ConsoleLogErrorWithTag("vulkan", "Atomic Current Frame Index = {}", g_CurrentFrameIndex.load());
        auto isResized = ApplicationInstance::GetWindow().GetCurrentSwapchain()->IsResized();
        // ConsoleLogTraceWithTag("vulkan", "IsResized (bool) = {}", isResized);
        if(isResized){
            // ConsoleLogInfoWithTag("vulkan", "How often are you being called!");
            ConsoleLogFatalWithTag("vulkan", "How often are you being called to be initialized?");
            // Renderer::Initialize();
            InitializeRendererPipeline();
        }

        VkCommandBufferBeginInfo cmd_buffer_begin_info = {
            .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
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
        clearValues[0].color = {{0.1f, 0.1f, 0.1f, 1.0f}};
        clearValues[1].depthStencil = {1.0f, 0};

        rp_begin_info.clearValueCount = static_cast<uint32_t>(clearValues.size());
        rp_begin_info.pClearValues = clearValues.data();


        vkCmdBeginRenderPass(cmd_buffer, &rp_begin_info, VK_SUBPASS_CONTENTS_INLINE);
        g_IsFrameStarted = true;
        
        VkViewport viewport = {
            .x = 0.0f,
            .y = 0.0f,
            .width = static_cast<float>(ApplicationInstance::GetWindow().GetCurrentSwapchain()->GetSwapchainExtent().width),
            .height = static_cast<float>(ApplicationInstance::GetWindow().GetCurrentSwapchain()->GetSwapchainExtent().height),
            .maxDepth = 1.0f,
        };

        VkRect2D scissor = {
            .offset = {0, 0},
            .extent = ApplicationInstance::GetWindow().GetCurrentSwapchain()->GetSwapchainExtent()
        };

        g_CommandBuffers[g_CurrentFrameIndex] = cmd_buffer;

        vkCmdSetViewport(g_CommandBuffers[g_CurrentFrameIndex], 0, 1, &viewport);
        vkCmdSetScissor(g_CommandBuffers[g_CurrentFrameIndex], 0, 1, &scissor);
    }

    void VulkanRenderer::EndFrame(){
        auto cmd_buffer = GetCurrentCommandBuffer();
        vkCmdEndRenderPass(cmd_buffer);
        vk::vk_check(vkEndCommandBuffer(cmd_buffer), "vkEndCommandBuffer", __FILE__, __LINE__, __FUNCTION__);

        ApplicationInstance::GetWindow().GetCurrentSwapchain()->SubmitCommandBuffer(&cmd_buffer);
        g_IsFrameStarted = false;
    }


    void VulkanRenderer::DrawScene(Ref<SceneObject> p_SceneContext) {
        if(!p_SceneContext){}
    }

    void VulkanRenderer::DrawSceneObjects(const Ref<SceneScope>& p_Scene){
        // Draw Scene Objects (In the view of the frustrum)
        // ConsoleLogInfo("DrawSceneObjects() called!");
        // if(!p_SceneObjects.contains("")){}

        if(!p_Scene){}
        // auto& entites = p_Scene->Get

        // auto current_cmd_buffer = GetCurrentCommandBuffer();

        // vkCmdBindPipeline(current_cmd_buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, g_Shader->GetGraphicsPipeline());
        // // float delta_time = SyncUpdate::DeltaTime();

        // // ConsoleLogInfo("DrawSceneObjects() called! #2");
        
        // // Setting up camera
        // auto& camera = p_SceneObjects["camera"];
        // auto& camera_pos = camera->GetComponent<Transform>().Position;
        // auto& camera_component = camera->GetComponent<Camera>();
        // auto camera_proj_view = camera_component.GetProjection() * camera_component.GetView();
        // // ConsoleLogInfo("DrawSceneObjects() called! #3");

        // // setting up one object
        // auto& object = p_SceneObjects["player"];
        
        // PushConstantData push_data = {
        //     .Transform = camera_proj_view * object->toMat4(),
        //     .Model = object->toMat4(),
        //     .LightTransform = camera_pos - object->GetComponent<Transform>().Position
        // };

        // vkCmdPushConstants(
        //     current_cmd_buffer,
        //  g_PipelineLayout,
        //   VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT,
        //    0,
        //     sizeof(PushConstantData),
        //     &push_data
        // );

        // ConsoleLogInfo("DrawSceneObjects() called! #5");

        // auto& player_mesh = object->GetComponent<MeshComponent>();
        // // if(player_mesh.MeshMetadata){
        // //     ConsoleLogFatal("MeshComponent metadata is not valid!!");
        // // }
        // // ConsoleLogInfo("DrawSceneObjects() called! #5.1");
        // auto& vb = player_mesh.MeshMetadata.GetVertices();
        // // ConsoleLogInfo("DrawSceneObjects() called! #5.2");
        // auto ib = player_mesh.MeshMetadata.GetIndices();
        // // ConsoleLogInfo("DrawSceneObjects() called! #6");

        // vb->Bind(current_cmd_buffer);

        // // ConsoleLogInfo("DrawSceneObjects() called! #7");

        // if(ib != nullptr){
        //     ib->Bind(current_cmd_buffer);
        //     ConsoleLogInfo("Binding Index Buffer!");
        //     // ConsoleLogInfo("DrawSceneObjects() called! #8");
        //     if(ib->HasIndicesPresent()){
        //         // ConsoleLogInfo("DrawSceneObjects() called! #9");
        //         ib->Draw(GetCurrentCommandBuffer());
        //     }
        //     else{
        //         ConsoleLogInfo("DrawSceneObjects() called! #9");
        //         vb->Draw(GetCurrentCommandBuffer());
        //     }
        // }
        // else{
        //     ConsoleLogInfo("DrawSceneObjects() called! #8");
        //     vb->Draw(GetCurrentCommandBuffer());
        // }
    }

    void VulkanRenderer::DrawSceneObject(Ref<SceneObject>& p_CurrentObject){
        if(p_CurrentObject == nullptr){}
        // auto current_cmd_buffer = GetCurrentCommandBuffer();

        // vkCmdBindPipeline(current_cmd_buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, g_Shader->GetGraphicsPipeline());

        // PushConstantData data = {
        // };
    }

    void VulkanRenderer::DrawObjectWithCamera(Ref<SceneObject>& p_Object, Ref<SceneObject>& p_CameraObject){
        auto current_cmd_buffer = GetCurrentCommandBuffer();
        
        vkCmdBindPipeline(current_cmd_buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, g_Shader->GetGraphicsPipeline());
        // auto camera_transform = p_CameraObject->GetComponent<Transform>();
        // auto camera_component = p_CameraObject->GetComponent<Camera>();
        auto* camera_component = p_CameraObject->GetComponent<engine3d::PerspectiveCamera>();

        // auto camera_position = camera_transform->Position;
        // auto camera_position = camera_component->Position;
        // auto& projection = camera_component.GetProjection();
        // auto proj_view = camera_component->GetProjection() * camera_component->GetView();
        auto obj_model = p_Object->GetModelMatrix();

        // auto front_position_of_camera = camera_component->Front;
        // p_Object->GetComponent<engine3d::Transform>()


        float delta_time = SyncUpdate::DeltaTime();
        PushConstantData push_const_data = {
            .Projection = camera_component->GetProjection(),
            .View = camera_component->GetView(),
            .Model = obj_model,
            // .LightTransform = front_position_of_camera * delta_time
            .LightTransform = camera_component->Position * delta_time
            // .LightTransform = (front_position_of_camera - p_Object->GetComponent<engine3d::Transform>()->Position) * delta_time
            // .LightTransform = p_Object->GetComponent<Transform>()->Position
            // .LightTransform = camera_position - p_Object->GetComponent<Transform>()->Position
        };

        vkCmdPushConstants(
            current_cmd_buffer,
            g_PipelineLayout,
            VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT,
            0,
            sizeof(PushConstantData),
            &push_const_data
        );

        // const MeshComponent* component = p_Object->GetComponent<MeshComponent>();
        SceneObject& object = *p_Object;

        auto* component = object.GetComponent<MeshComponent>();
        if(component == nullptr){
            ConsoleLogFatal("MeshComponent was invalid that was attempted to get in vulkan_renderer!!!");
            return;
        }
        Mesh mesh_data = component->MeshMetadata;

        // if(mesh_data.IsLoaded()){
        //     ConsoleLogInfo("Mesh Data was indeed loaded!!!");
        // }

        auto& vb = mesh_data.GetVertices();
        auto ib = mesh_data.GetIndices();


        vb->Bind(current_cmd_buffer);

        if(ib != nullptr){
            ib->Bind(current_cmd_buffer);
 
            if(ib->HasIndicesPresent()){
                // ConsoleLogInfo("Ib->Draw being called!");
                ib->Draw(current_cmd_buffer);
            }
            else{
                vb->Draw(current_cmd_buffer);
            }
        }
        else{
            vb->Draw(current_cmd_buffer);
        }
    }

    VkCommandBuffer VulkanRenderer::GetCurrentCommandBuffer(){
        return g_CommandBuffers[g_CurrentFrameIndex];
    }

    VkCommandBuffer VulkanRenderer::CurrentCommandBuffer(){
        return g_CommandBuffers[g_CurrentFrameIndex];
    }

    VkFramebuffer VulkanRenderer::CurrentFramebuffer(){
        return ApplicationInstance::GetWindow().GetCurrentSwapchain()->GetFramebuffer(g_CurrentFrameIndex);
    }

    uint32_t VulkanRenderer::GetCurrentCommandBufferIndex(){
        return g_CurrentFrameIndex;
    }
};