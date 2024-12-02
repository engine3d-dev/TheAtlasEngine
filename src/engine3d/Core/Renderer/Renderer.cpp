#include <Core/EngineLogger.hpp>
#include <Core/GraphicDrivers/Shader.hpp>
#include <Core/TimeManagement/UpdateManagers/SyncUpdateManager.hpp>
#include "GraphicDrivers/UniformBuffer.hpp"
#include "internal/Vulkan2Showcase/Shaders/VulkanShader.hpp"
#include "internal/Vulkan2Showcase/VulkanContext.hpp"
#include "internal/Vulkan2Showcase/VulkanSwapchain.hpp"
#include "internal/Vulkan2Showcase/helper_functions.hpp"
#include <Core/ApplicationInstance.hpp>
#include <Core/Renderer/Renderer.hpp>
#include <glm/geometric.hpp>
#include <numeric>
#include <vulkan/vulkan_core.h>
#include <vector>

#include <Core/SceneManagment/Components/SPComps/Transform.hpp>
#include <array>
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

#include <Core/SceneManagment/Components/SPComps/Camera.hpp>

#include <Core/internal/Vulkan2Showcase/VulkanDescriptors.hpp>

namespace engine3d{
    //! @note Containing relevant frame information to be provided to the renderer
    //! @note In the tutorial, he uses this struct to contain properties such as camera, command buffer, etc to keep track of the data used throughout the renderer.
    //! @note For our use-case we may not want to do it this way, and it is going to change soon....
    //! @note Leaving it here though, for reference.
    // struct FrameInfo{
    //     int FrameIndex;
    //     float FrameTime;
    //     VkCommandBuffer CommandBuffer;
    // };

    static std::vector<VkCommandBuffer> g_CommandBuffers;

    VkCommandPool g_CmdPool;
    //! @note Keeping track of our current frames in flight at rendering.
    uint32_t g_CurrentFrameIndex = -1;
    static bool g_IsFrameStarted = false;

    static Ref<Shader> g_Shader = nullptr;
    static Ref<Shader> g_UboShaderTutorial = nullptr;
    static VkPipelineLayout g_PipelineLayout;
    static VkPipeline g_Pipeline;

    //! TODO: UniformBuffer will be initialized here, but should be moved out of the renderer into some mesh struct or something like that.
    // static UniformBuffer s_Ubo;

    //! @note Push Constants to apply data to the shaders.
    //! @note vk::VulkanModal is how shaders are going to be 
    struct SimplePushConstantData{
        glm::mat4 Transform{1.f};
        glm::mat4 ModelMatrix{1.f}; // I think this replaced model matrix.
        glm::vec3 LightTransform{1.0, -3.0, -1.0};
        // alignas(16) glm::vec3 Color;
    };

    //! @note This is the tutorial's push const data struct
    struct OldSimplePushConstantData{
        glm::mat4 Transform{1.f};
        glm::mat3 ModelMatrix{1.f}; // Model Matrix
        glm::vec3 LightTransform{1.0f, -3.0f, -1.0f};
    };

    struct GlobalUbo {
        glm::mat4 ProjectionView{1.f};
        // glm::vec3 LightDirection = glm::normalize(glm::vec3{1.f, -3.f, -1.f});
        // glm::vec3 DirectionToLight{1.0f, -3.0f, -1.0f}; // Dir To Light Transform
        glm::vec4 AmbientLightColor = {1.f, 1.f, 1.f, .02f}; // w = intensity
        glm::vec3 LightPosition{-1.f};
        alignas(16) glm::vec4 LightColor{1.f}; // w - is light intensity
    };

    // *********************************************************************
    //! @note NEW: We have uniform buffers that correspond to the index.
    static std::vector<UniformBuffer> g_UniformBuffers;
    // static UniformBuffer g_Ubo;

    //! @note NEW: Descriptor Stuff....
    static Scope<vk::VulkanDescriptorPool> g_GlobalPool;

    //! @note NEW: In tutorial VkDescriptrSet globalDescriptorSet is added to FrameInfo. Because we dont have a FrameInfo (am doing it differently), we just put it here.
    static VkDescriptorSet g_GlobalDescriptorSet;
    static std::vector<VkDescriptorSetLayout> g_DescriptorSetLayoutVector;

    static std::vector<VkDescriptorSet> g_Global_desc_sets;
    // *********************************************************************

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

        
        //! @note NEW: Adding the descriptors here
        //! @note This was added after adding uniform buffers.
        g_GlobalPool = vk::VulkanDescriptorPool::Builder()
                        .SetMaxSets(vk::VulkanSwapchain::MaxFramesInFlight)
                        .AddPoolSize(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, vk::VulkanSwapchain::MaxFramesInFlight)
                        .Build();
        

        auto glob_set_layout = vk::VulkanDescriptorSetLayout::Builder()
                               .AddBinding(0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_VERTEX_BIT)
                               .Build();

        // *******************************************************
        // ***************[Creating Pipeline Layout]**************
        // *******************************************************
        //! @note New after adding descriptor set layouts
        g_DescriptorSetLayoutVector = std::vector<VkDescriptorSetLayout>{glob_set_layout->GetDescriptorSetLayout()};


        //! @note First initializing pipeline layout create info
        VkPipelineLayoutCreateInfo pipeline_layout_create_info = {
            .sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
            .pNext = nullptr,
            .flags = 0,
            // .setLayoutCount = 0,
            // .pSetLayouts = nullptr,
            .setLayoutCount = static_cast<uint32_t>(g_DescriptorSetLayoutVector.size()),
            .pSetLayouts = g_DescriptorSetLayoutVector.data(),
            .pushConstantRangeCount = 1,
            .pPushConstantRanges = &push_const_range
        };

        vk::vk_check(vkCreatePipelineLayout(vk::VulkanContext::GetDriver(), &pipeline_layout_create_info, nullptr, &g_PipelineLayout), "vkCreatePipelineLayout", __FILE__, __LINE__, __FUNCTION__);
        
        // *******************************************************
        // ****************[End of Pipeline Layout]**************
        // *******************************************************

        //! @note We are setting our shader pipeline to utilize our current window's swapchain
        //! @note a TODO is to utilize different render passes utiization for shader pipelines, potentially.
        pipeline_config.PipelineRenderPass = ApplicationInstance::GetWindow().GetCurrentSwapchain()->GetRenderPass();
        pipeline_config.PipelineLayout = g_PipelineLayout;

        // m_Shader = Shader::Create("simple_shader/simple_shader.vert.spv", "simple_shader/simple_shader.frag.spv", pipeline_config);
        g_Shader = Shader::Create("showcase_shaders/simple_shader.vert.spv", "showcase_shaders/simple_shader.frag.spv", pipeline_config);
        g_UboShaderTutorial = Shader::Create("sim_shader_ubo_tutorial/simple_shader.vert.spv", "sim_shader_ubo_tutorial/simple_shader.frag.spv", pipeline_config);

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


        //! @note Setting up Uniform Buffers here
        //! TODO: Move uniform buffers...
        //! @note Bandage fix for NonCoherentAtomSize bug.


        auto phys_driver_props = vk::VulkanContext::GetPhysicalDriver().GetProperties();
        auto min_offset_alignment = std::lcm(phys_driver_props.limits.minUniformBufferOffsetAlignment, phys_driver_props.limits.nonCoherentAtomSize);

        //! @note We want their to be the same amount of uniform buffers as our current frames in flight.
        //! @note Uniform buffers are how we are going to be sending data to our pipelines.
        ConsoleLogInfo("Renderer --- Begin Initializing Uniform Buffers at Max_Frames_In_Flight = {}", vk::VulkanSwapchain::MaxFramesInFlight);
        g_UniformBuffers = std::vector<UniformBuffer>(vk::VulkanSwapchain::MaxFramesInFlight);
        ConsoleLogTrace("g_UniformBuffers.size() === {}", g_UniformBuffers.size());
        // uint32_t instance_count = vk::VulkanSwapchain::MaxFramesInFlight;
        uint32_t instance_count = 1;
        for(int i = 0; i <= g_UniformBuffers.size(); i++){
            g_UniformBuffers[i] = UniformBuffer(
                sizeof(GlobalUbo),
                instance_count,
                VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
                VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT,
                min_offset_alignment
            );
            
            //! @note In tutorial referenced to as .map(), I just changed it to be explicitly called MapData
            g_UniformBuffers[i].MapData();
        }

        ConsoleLogWarn("Renderer --- g_UniformBuffers Initialized Correctly!!");
        
        g_Global_desc_sets = std::vector<VkDescriptorSet>(vk::VulkanSwapchain::MaxFramesInFlight);

        for(int i = 0; i <= g_Global_desc_sets.size(); i++){
            auto buffer_info = g_UniformBuffers[i].InitializeDescriptorInfo();
            vk::VulkanDescriptorWriter(*glob_set_layout, *g_GlobalPool)
            .WriteBuffer(0, &buffer_info)
            .Build(g_Global_desc_sets[i]);
        }



        ConsoleLogInfo("CommandBuffers Size === {}", g_CommandBuffers.size());
    }

    void Renderer::SetBackgroundColor(const std::array<float, 4>& p_Rgba){
        VkClearValue clearColorValue = {{p_Rgba[0], p_Rgba[1], p_Rgba[2], p_Rgba[3]}};
    }

    void Renderer::BeginFrame(){
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

    void Renderer::RecordSceneGameObjects(std::vector<SceneObject*>& p_Objects, SceneObject* p_CameraObject){
        auto current_cmd_buffer = GetCurrentCommandBuffer();

        //! @note Essentially doing m_Pipeline->Bind(m_CommandBuffer[i])
        //! @note Starts when to start rendering!!
        vkCmdBindPipeline(current_cmd_buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, g_Shader->GetGraphicsPipeline());
        float delta_time = SyncUpdateManager::GetInstance()->m_SyncLocalDeltaTime;
        auto camera_component = p_CameraObject->GetComponent<Camera>();
        
        //! @note Only for testing purposes for mesh data.
        // auto point_light_obj = p_Objects[2];
        // auto point_light_position = point_light_obj->GetComponent<Transform>().m_Position;
        
        // for(size_t i = 0; i <= p_Objects.size()-1; i++){
            // ConsoleLogWarn("Index (i <= 1) = {}", i);
        for(const auto& obj : p_Objects){
            // auto obj = p_Objects[i];

            auto proj_view = camera_component.GetProjection() * camera_component.GetView();

            auto model_matrix = obj->toMat4();

            SimplePushConstantData push = {
                .Transform = proj_view * model_matrix,
                .ModelMatrix = model_matrix,
                // .LightTransform = point_light_position
            };

            vkCmdPushConstants(
                current_cmd_buffer,
                g_PipelineLayout,
                VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT,
                0,
                    sizeof(SimplePushConstantData), 
                    &push
            );

            auto& vb = obj->GetMesh().GetVertices();
            auto ib = obj->GetMesh().GetIndices();
            vb->Bind(current_cmd_buffer);

            if(ib != nullptr){
                ib->Bind(current_cmd_buffer);
                if(ib->HasIndicesPresent()){
                    ib->Draw(GetCurrentCommandBuffer());
                }
                else{
                    vb->Draw(GetCurrentCommandBuffer());
                }
            }
            else{
                vb->Draw(GetCurrentCommandBuffer());
            }

        }
    }

    void Renderer::RecordSceneGameObjects(std::unordered_map<std::string, std::vector<SceneObject*>>& p_AllSceneObjects){

        auto current_cmd_buffer = GetCurrentCommandBuffer();

        //! @note Essentially doing m_Pipeline->Bind(m_CommandBuffer[i])
        //! @note Starts when to start rendering!!
        vkCmdBindPipeline(current_cmd_buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, g_Shader->GetGraphicsPipeline());
        float delta_time = SyncUpdateManager::GetInstance()->m_SyncLocalDeltaTime;
        auto cameraObject = p_AllSceneObjects["Cameras"][0];
        auto camera_component = cameraObject->GetComponent<Camera>();
        //! @note Only for testing purposes for mesh data.
        // auto point_light_obj = p_Objects[2];
        // auto point_light_position = point_light_obj->GetComponent<Transform>().m_Position;
        auto& position = p_AllSceneObjects["Cameras"][0]->GetComponent<Transform>().m_Position;

        for(const auto& obj : p_AllSceneObjects.at("RenderedObjects")){

            auto proj_view = camera_component.GetProjection() * camera_component.GetView();

            auto model_matrix = obj->toMat4();

            SimplePushConstantData push = {
                .Transform = proj_view * model_matrix,
                .ModelMatrix = model_matrix,
                .LightTransform = position - obj->GetComponent<Transform>().m_Position
                // .LightTransform = position
            };

            vkCmdPushConstants(
                current_cmd_buffer,
                g_PipelineLayout,
                VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT,
                0,
                    sizeof(SimplePushConstantData), 
                    &push
            );
            auto& vb = obj->GetMesh().GetVertices();
            auto ib = obj->GetMesh().GetIndices();
            vb->Bind(current_cmd_buffer);
            if(ib != nullptr){
                ib->Bind(current_cmd_buffer);
                if(ib->HasIndicesPresent()){
                    ib->Draw(GetCurrentCommandBuffer());
                }
                else{
                    vb->Draw(GetCurrentCommandBuffer());
                }
            }
            else{
                vb->Draw(GetCurrentCommandBuffer());
            }

        }
    }


    void Renderer::RecordSceneGameObjectsWithUniformBuffers(std::unordered_map<std::string, std::vector<SceneObject*>>& p_AllSceneObjects){
        auto current_cmd_buffer = GetCurrentCommandBuffer();

        //! @note Essentially doing m_Pipeline->Bind(m_CommandBuffer[i])
        //! @note Starts when to start rendering!!

        //! @note Utilizing our shader pipeline here for the ubo/descriptor sets tutorial.
        vkCmdBindPipeline(current_cmd_buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, g_UboShaderTutorial->GetGraphicsPipeline());
        float delta_time = SyncUpdateManager::GetInstance()->m_SyncLocalDeltaTime;
        auto cameraObject = p_AllSceneObjects["Cameras"][0];
        auto camera_component = cameraObject->GetComponent<Camera>();
        
        // Calling point light
        auto point_light_transform = p_AllSceneObjects["PointLights"][0]->GetComponent<Transform>();


        //! @note Only for testing purposes for mesh data.
        auto& position = p_AllSceneObjects["Cameras"][0]->GetComponent<Transform>().m_Position;

        //! @note Bind descriptor sets per frame (frames-in-flight)
        vkCmdBindDescriptorSets(
            current_cmd_buffer, 
            VK_PIPELINE_BIND_POINT_GRAPHICS, 
            g_PipelineLayout,
            0,
            1,
            &g_Global_desc_sets[g_CurrentFrameIndex],
            0,
            nullptr
        );

        for(const auto& obj : p_AllSceneObjects.at("RenderedObjects")){
            auto proj_view = camera_component.GetProjection() * camera_component.GetView();
            auto model_matrix = obj->toMat4();

            /*
            // Using this for reference
            SimplePushConstantData push = {
                .Transform = proj_view * model_matrix,
                .ModelMatrix = model_matrix,
                .LightTransform = position - obj->GetComponent<Transform>().m_Position
            };
            */

            //! @note This is new here.
            //! @note We are writing our buffer data to the uniform buffer, and specifying at the
            //        current frame we are on this uniform buffer is being written to.
            //! @note Uniform buffer data that gets used by the descriptor set that wont get updated as continuously as push constants
            GlobalUbo ubo{
                .ProjectionView = proj_view,
                .LightPosition = {point_light_transform.m_Position.x, point_light_transform.m_Position.y, point_light_transform.m_Position.z},
                .LightColor = {1.f, 0.f, 0.f, 1.f}
                // .DirectionToLight = position - obj->GetComponent<Transform>().m_Position,
                // .ProjectionView = proj_view
            };

            g_UniformBuffers[g_CurrentFrameIndex].CopyTo(&ubo);
            g_UniformBuffers[g_CurrentFrameIndex].Flush();

            //! @note Push constanst for data we want to continously update
            SimplePushConstantData push = {
                .Transform = proj_view * model_matrix,
                .ModelMatrix = model_matrix,
                // .LightTransform = position - obj->GetComponent<Transform>().m_Position
                // .LightTransform = position
            };

            
            vkCmdPushConstants(
                current_cmd_buffer,
                g_PipelineLayout,
                VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT,
                0,
                    sizeof(SimplePushConstantData), 
                    &push
            );
            


            //! @note This makes sure our meshes gets rendered utilizing their vertices and indices.
            //! @note While handling our edge cases.
            auto& vb = obj->GetMesh().GetVertices();
            auto ib = obj->GetMesh().GetIndices();
            vb->Bind(current_cmd_buffer);
            if(ib != nullptr){
                ib->Bind(current_cmd_buffer);
                if(ib->HasIndicesPresent()){
                    ib->Draw(GetCurrentCommandBuffer());
                }
                else{
                    vb->Draw(GetCurrentCommandBuffer());
                }
            }
            else{
                vb->Draw(GetCurrentCommandBuffer());
            }

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