#include "drivers/ui/imgui_backend.hpp"
#include "scene/scene.hpp"
#include <cstring>
#include <deque>
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
#include <core/update_handlers/sync_update.hpp>

#include <drivers/vulkan/descriptor_tutorial.hpp>
#include <drivers/vulkan/buffer_tutorial.hpp>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <vector>
#include <core/image/stb_image.hpp>

#include <core/event/event.hpp>

namespace atlas::vk{
    static std::vector<VkCommandBuffer> g_command_buffers;
    VkCommandPool g_command_pool;
    std::atomic<uint32_t> g_current_frame_index = -1;
    static bool g_is_frame_started = false;
    static VkPipelineLayout g_pipeline_layout;
    static std::vector<ref<scene_object>> g_scene_object_queue;

    static std::deque<std::function<void()>> s_main_deletion_queue;
    static VkDevice g_driver = nullptr;

    static ref<descriptor_pool> g_global_pool;
    inline std::vector<BufferTutorial> g_global_ubo_list;
    static scope<descriptor_set_layout> g_global_set_layout;
    static std::vector<VkDescriptorSet> g_global_descriptor_set;

    /**
     * @name uniform_buffer
     * @note TODO: Should probably be its own class that also enables to read/write to it to the shaders
    */
    struct uniform_buffer{
        VkBuffer BufferHanlder;
        VkDeviceMemory BufferMemory;
        void* uniform_bufferMappedData;
    };


    // static ref<scene_object> g_CurrentSelectedCamera;
    template<typename UFunction>
    void SubmitResourceFree(const UFunction& p_task){
        s_main_deletion_queue.push_back(p_task);
    }

    //! @note Shader-definitions
    static ref<shader> g_shader = nullptr;
    //! @note Since descriptor sets aren't working, we are creating another shader just to set the colors lol


    struct camera_ubo{
        glm::mat4 Projection{1.f};
        glm::mat4 View{1.f};
        glm::mat4 Model{1.f};
        glm::vec3 LightPosition{1.0, -3.0, -1.0};
        glm::vec4 Color{1.f};
        glm::vec2 MousePosition{0.f};
    };

    struct PointLighTest{
        glm::vec3 Position;
        glm::vec4 Ambient;
        glm::vec3 Color;
    };

    void createBuffer(VkDeviceSize p_size, VkBufferUsageFlags p_usage, VkMemoryPropertyFlags p_properties, VkBuffer& p_buffer, VkDeviceMemory& p_buffer_memory){
		VkBufferCreateInfo bufferInfo{};
		bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		bufferInfo.size = p_size;
		bufferInfo.usage = p_usage;
		bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;


        vk_check(vkCreateBuffer(vk_context::get_current_driver(), &bufferInfo, nullptr, &p_buffer), "vkCreateBuffer", __FILE__, __LINE__, __FUNCTION__);

		VkMemoryRequirements memRequirements;
		vkGetBufferMemoryRequirements(vk_context::get_current_driver(), p_buffer, &memRequirements);

		VkMemoryAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		allocInfo.allocationSize = memRequirements.size;
		// allocInfo.memoryTypeIndex = findMemoryType(memRequirements.memoryTypeBits, properties);
        allocInfo.memoryTypeIndex = vk_context::get_current_selected_physical_driver().search_memory_type(memRequirements.memoryTypeBits, p_properties);

		vk_check(vkAllocateMemory(vk_context::get_current_driver(), &allocInfo, nullptr, &p_buffer_memory), "vkAllocateMemory", __FILE__, __LINE__, __FUNCTION__);

		vkBindBufferMemory(vk_context::get_current_driver(), p_buffer, p_buffer_memory, 0);
	}
    
    void vk_renderer::initialize_pipeline(){
        auto max_frames_in_flight = vk_swapchain::MaxFramesInFlight;
        g_driver = vk_context::get_current_driver();

        g_global_pool = descriptor_pool::builder()
                       .setMaxSets(max_frames_in_flight)
                       .addPoolSize(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, max_frames_in_flight)
                       .addPoolSize(VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, max_frames_in_flight)
                       .build();


        g_global_set_layout = descriptor_set_layout::builder()
                                .addBinding(0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_VERTEX_BIT)
                                .build();

        g_global_descriptor_set = std::vector<VkDescriptorSet>(max_frames_in_flight);
        
        g_global_ubo_list.resize(max_frames_in_flight);

        for(size_t i = 0; i < g_global_ubo_list.size(); i++){
            g_global_ubo_list[i] = BufferTutorial(sizeof(camera_ubo), 1, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);
            g_global_ubo_list[i].map();
        }



        for(size_t i = 0; i < g_global_descriptor_set.size(); i++){
            auto buffer_info = g_global_ubo_list[i].descriptor_info();

            descriptor_writer(*g_global_set_layout, *g_global_pool)
            .writeBuffer(0, &buffer_info)
            .build(g_global_descriptor_set[i]);
        }

        //! @note Setting up push constants
        //! @note VkPipelineLayoutCreateInfo does not require a push constant to be defined in it's configuration
        //! @note It can just be set to its default vulkan has its configuration
        g_current_frame_index = 0;
        g_is_frame_started = false;

        //! @note Setting up our pipeline.
        auto pipeline_config = vk::vk_shader::shader_configuration(application_instance::get_window().get_width(), application_instance::get_window().get_height());

        //! @note Initialize Push constant range

        VkPushConstantRange push_const_range = {
            .stageFlags =  VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT,
            .offset = 0,
            .size = sizeof(camera_ubo)
        };

        //! @note We are setting our descriptors to work with layout(set = 0, binding = 0)
        // std::vector<VkDescriptorSetLayout> setLayouts;
        // setLayouts.push_back(g_global_set_layout->get_descriptor_set_layout());

        //! @note First initializing pipeline layout create info
        //! @note TODO: Handle if a pipeline layout has descriptors or no
        /*
        
            Pipeline API would look like the following

            Pipeline pipeline_a = Pipeline(descriptors: false, descriptors_count = 0, descriptors: std::span<DescriptorSet>);
            
        */

        std::vector<VkDescriptorSetLayout> descriptor_set_layouts = {g_global_set_layout->get_descriptor_set_layout()};

        VkPipelineLayoutCreateInfo pipeline_layout_create_info = {
            .sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
            .pNext = nullptr,
            .flags = 0,
            // .setLayoutCount = 0,
            // .pSetLayouts = nullptr,
            .setLayoutCount = static_cast<uint32_t>(descriptor_set_layouts.size()),
            .pSetLayouts = descriptor_set_layouts.data(),
            // .setLayoutCount = 1,
            // .pSetLayouts = &g_DescriptorSetLayout,
            // .setLayoutCount = 1,
            // .pSetLayouts = &info.Layout,
            .pushConstantRangeCount = 1,
            .pPushConstantRanges = &push_const_range
        };

        vk::vk_check(vkCreatePipelineLayout(g_driver, &pipeline_layout_create_info, nullptr, &g_pipeline_layout), "vkCreatePipelineLayout", __FILE__, __LINE__, __FUNCTION__);


        //! @note We are setting our shader pipeline to utilize our current window's swapchain
        //! @note a TODO is to utilize different render passes utiization for shader pipelines, potentially.
        pipeline_config.PipelineRenderPass = application_instance::get_window().get_current_swapchain()->get_renderpass();
        pipeline_config.PipelineLayout = g_pipeline_layout;

        // m_Shader = shader::create("simple_shader/simple_shader.vert.spv", "simple_shader/simple_shader.frag.spv", pipeline_config);
        g_shader = shader::create("shaders/mouse_picking_shaders/simple_shader.vert.spv", "shaders/mouse_picking_shaders/simple_shader.frag.spv", pipeline_config);
        // g_ColorShader = shader::create("shader_ubo_tutorial/simple_shader.vert.spv", "shader_ubo_tutorial/simple_shader.frag.spv", pipeline_config);
        
        // if()
        console_log_error("NOT AN ERROR: Shader Loaded Successfully!");


        //! @note Initializing Command buffers.
        g_command_buffers.resize(application_instance::get_window().get_current_swapchain()->get_images_size());

        VkCommandPoolCreateInfo pool_create_info = {
            .sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
            .pNext = nullptr,
            .flags = VK_COMMAND_POOL_CREATE_TRANSIENT_BIT | VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT,
            .queueFamilyIndex = vk::vk_context::get_current_selected_physical_driver().get_queue_indices().Graphics
        };

        vk::vk_check(vkCreateCommandPool(g_driver, &pool_create_info, nullptr, &g_command_pool), "vkCreateCommandPool", __FILE__, __LINE__, __FUNCTION__);


        console_log_info("RENDERER COMMAND BUFFERS SIZE === {}", g_command_buffers.size());
        //! @note Allocating our command buffers.
        VkCommandBufferAllocateInfo cmd_buffer_alloc_info = {
            .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
            .commandPool = g_command_pool,
            .level = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
            .commandBufferCount = static_cast<uint32_t>(g_command_buffers.size()),
        };

        vk::vk_check(vkAllocateCommandBuffers(g_driver, &cmd_buffer_alloc_info, g_command_buffers.data()), "vkAllocateCommandBuffers", __FILE__, __LINE__, __FUNCTION__);

        console_log_info("CommandBuffers Size === {}", g_command_buffers.size());
    }

    void vk_renderer::set_initial_camera(ref<scene_object>& camera){
        // g_CurrentSelectedCamera = camera;
        camera = {};
    }

    vk_renderer::vk_renderer(const std::string& p_tag){
        g_current_frame_index = 0;
        g_is_frame_started = false;
        if(p_tag.empty()){}
        g_scene_object_queue = std::vector<ref<scene_object>>();

        // Initializing Uniform Buffers
        // Createuniform_buffers();

        // initializing descriptors
        // CreateDescriptorSetLayout();

        initialize_pipeline();
    }

    void CleanupPipeline(){
        vkDestroyPipelineLayout(g_driver, g_pipeline_layout, nullptr);
    }

    void vk_renderer::begin_frame(){
        
        if(swapchain::is_rebuild()){
            console_log_fatal("ARE YOU CALED AFTER RESIZE?!?!");
            initialize_pipeline();
            swapchain::reset_build();
        }

        //! @note Note to Self: This is just a hack on how to get "resizing" to work
        //! @note I will need to do this better as there are problems with this.
        //! @note Currently the state is if the window is resized by glfwSetFramebufferSizeCallback in vulkan_window.cpp, we also check here for the state of our swapchain.
        //! @note If our swapchain's state is either VK_ERROR_OUT_OF_DATE_KHR or VK_SUBOPTIMAL_KHR, then the Acquired function returns -3.
        //! @note -3 was just to indicate the swapchain's state as been modified and needs to modify the pipeline to work with the new swapchain.
        //! @note Another TODO: Make abstract the pipeline's so that we do not have to modify this directly.
        //! @note If this isn't already done, but the vk::submit function should handle the way we directly handle these tasks... potentially?
        g_current_frame_index = application_instance::get_window().get_current_swapchain()->acquire_next_image();
        if(swapchain::is_rebuild()){
            console_log_fatal_tagged("vulkan", "How often are you being called to be initialized?");
            // renderer::Initialize();
            initialize_pipeline();
        }

        VkCommandBufferBeginInfo cmd_buffer_begin_info = {
            .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
        };

        auto cmd_buffer = get_current_command_buffer();

        vk::vk_check(vkBeginCommandBuffer(cmd_buffer, &cmd_buffer_begin_info), "vkBeginCommandBuffer", __FILE__, __LINE__, __FUNCTION__);
        VkRenderPassBeginInfo rp_begin_info = {
            .sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO,
            .renderPass = application_instance::get_window().get_current_swapchain()->get_renderpass(),
            .framebuffer = application_instance::get_window().get_current_swapchain()->get_framebuffer(g_current_frame_index), // Specifying which framebuffer to render pass to.
            .renderArea = {
                .offset = {0, 0},
                .extent = {application_instance::get_window().get_width(), application_instance::get_window().get_height()}
            }
        };

        std::array<VkClearValue, 2> clearValues;
        clearValues[0].color = {{0.1f, 0.1f, 0.1f, 1.0f}};
        clearValues[1].depthStencil = {1.0f, 0};

        rp_begin_info.clearValueCount = static_cast<uint32_t>(clearValues.size());
        rp_begin_info.pClearValues = clearValues.data();


        vkCmdBeginRenderPass(cmd_buffer, &rp_begin_info, VK_SUBPASS_CONTENTS_INLINE);
        g_is_frame_started = true;
        
        VkViewport viewport = {
            .x = 0.0f,
            .y = 0.0f,
            .width = static_cast<float>(application_instance::get_window().get_current_swapchain()->get_extent().width),
            .height = static_cast<float>(application_instance::get_window().get_current_swapchain()->get_extent().height),
            .maxDepth = 1.0f,
        };

        VkRect2D scissor = {
            .offset = {0, 0},
            .extent = application_instance::get_window().get_current_swapchain()->get_extent()
        };

        g_command_buffers[g_current_frame_index] = cmd_buffer;

        vkCmdSetViewport(g_command_buffers[g_current_frame_index], 0, 1, &viewport);
        vkCmdSetScissor(g_command_buffers[g_current_frame_index], 0, 1, &scissor);

        ImGuiBackend::Begin();
    }

    void vk_renderer::end_frame(){
        auto current_cmd_buffer = get_current_command_buffer();

        //! @note Flushing out scene objects
        
        ImGuiBackend::End();
        vkCmdEndRenderPass(current_cmd_buffer);
        vk::vk_check(vkEndCommandBuffer(current_cmd_buffer), "vkEndCommandBuffer", __FILE__, __LINE__, __FUNCTION__);

        application_instance::get_window().get_current_swapchain()->submit_command_buffer(&current_cmd_buffer);
        g_is_frame_started = false;
    }


    void vk_renderer::draw_scene([[maybe_unused]] ref<scene_object> p_scene_ctx) {
    }

    void vk_renderer::draw_scene_objects([[maybe_unused]] const ref<scene_scope>& p_scene_ctx){
    }

    void vk_renderer::draw_scene_object([[maybe_unused]] ref<scene_object>& p_current_scene_object_ctx){
    }

    void vk_renderer::submit_scene_object(ref<scene_object>& p_current_scene_object_ctx){
        auto current_cmd_buffer = vk_renderer::current_command_buffer();
        
        vkCmdBindPipeline(current_cmd_buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, g_shader->get_graphics_pipeline());
        // auto* camera_component = g_CurrentSelectedCamera->get<atlas::PerspectiveCamera>();
        PerspectiveCamera* camera_component = nullptr;

        auto obj_model = p_current_scene_object_ctx->get_model();

        float delta_time = sync_update::delta_time();

        camera_ubo push_const_data = {
            .Projection = camera_component->GetProjection(),
            .View = camera_component->GetView(),
            .Model = obj_model,
            .LightPosition = (camera_component->Position - p_current_scene_object_ctx->get<Transform>()->Position) * delta_time,
        };
        // auto frame_idx = get_current_command_buffer_index();

        // memcpy(g_uniform_buffers[frame_idx].uniform_bufferMappedData, &push_const_data, sizeof(push_const_data));

        vkCmdPushConstants(
            current_cmd_buffer,
            g_pipeline_layout,
            VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT,
            0,
            sizeof(camera_ubo),
            &push_const_data
        );

        g_scene_object_queue.push_back(p_current_scene_object_ctx);
    }
    /**
    TODO: Here are a few issues that I think is whats causing the second mesh to not be rendered

    1.) Because we are binding graphics pipeline. This means that our second mesh is probably getting overridden with the first mesh
        - Still need to look into why thats the case (???)
    
    
    */
    void vk_renderer::draw_object_with_camera(ref<scene_object>& p_object, ref<scene_object>& p_camera){
        auto current_cmd_buffer = get_current_command_buffer();
        
        vkCmdBindPipeline(current_cmd_buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, g_shader->get_graphics_pipeline());

        auto* camera_component = p_camera->get<atlas::PerspectiveCamera>();

        auto obj_model = p_object->get_model();

        camera_ubo push_const_data = {
            .Projection = camera_component->GetProjection(),
            .View = camera_component->GetView(),
            .Model = obj_model,
            .LightPosition = -camera_component->Front,
            .Color = p_object->get<Transform>()->Color,
            // .MousePosition = event::cursor_position()
            .MousePosition = {event::cursor_position().x / application_instance::get_window().get_width(), event::cursor_position().y / application_instance::get_window().get_height()}
        };

        // auto frame_idx = g_current_frame_index.load();

        // vkCmdBindDescriptorSets(
        //     current_cmd_buffer,
        //     VK_PIPELINE_BIND_POINT_GRAPHICS,
        //     g_pipeline_layout,
        //     0,
        //     1,
        //     &g_global_descriptor_set[frame_idx],
        //     0,
        //     nullptr
        // );

        // auto current_ubo = g_global_ubo_list[frame_idx];
        // current_ubo.WriteToBuffer(&push_const_data);
        // current_ubo.Flush();


        // g_global_ubo_list[frame_idx].WriteToIndex(&push_const_data, frame_idx);
        // g_global_ubo_list[frame_idx].FlushIndex(frame_idx);

        // SceneObjectProperties properties = {
        //     .Color = p_Object->get<Transform>()->Color
        // };
        // push_const_data.LightPosition = {};

        // auto& uniform_buffer = g_uniform_buffers.at(g_current_frame_index.load());
        /*
        auto& uniform_buffer = g_uniform_buffers[g_current_frame_index.load()];
        if(uniform_buffer.BufferHanlder != nullptr){
        }


        

        VkDescriptorBufferInfo bufferInfoWrite = {};
        bufferInfoWrite.buffer = uniform_buffer.BufferHanlder;
        bufferInfoWrite.offset = 0;
        bufferInfoWrite.range = sizeof(camera_ubo);

        VkWriteDescriptorSet descriptorWrite = {};
        descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        descriptorWrite.dstSet = g_DescriptorSet;
        descriptorWrite.dstBinding = 0; // Binding point in the shader
        descriptorWrite.dstArrayElement = 0;
        descriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        descriptorWrite.descriptorCount = 1;
        descriptorWrite.pBufferInfo = &bufferInfoWrite; // Connect to buffer

        vkUpdateDescriptorSets(g_driver, 1, &descriptorWrite, 0, nullptr);*/

        // if(g_driver != nullptr){
        //     console_log_fatal("g_driver != nullptr!");
        // }


        // g_GlobalUbo.WriteToIndex(&push_const_data, g_current_frame_index.load());
        // g_GlobalUbo.FlushIndex(g_current_frame_index.load());
        // g_global_ubo_list[g_current_frame_index.load()].WriteToBuffer(&push_const_data, sizeof(camera_ubo));
        // g_global_ubo_list[g_current_frame_index.load()].Flush(sizeof(camera_ubo));
        // g_global_ubo_list[g_current_frame_index.load()].WriteToBuffer(&push_const_data);
        // g_global_ubo_list[g_current_frame_index.load()].Flush();
        
        // g_global_ubo_list[g_current_frame_index.load()].WriteToIndex(&push_const_data, g_current_frame_index.load());
        // g_global_ubo_list[g_current_frame_index.load()].FlushIndex(g_current_frame_index.load());
        
        vkCmdPushConstants(
            current_cmd_buffer,
            g_pipeline_layout,
            VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT,
            0,
            sizeof(camera_ubo),
            &push_const_data
        );

        g_scene_object_queue.push_back(p_object);
        
        scene_object& object = *p_object;

        auto* component = object.get<MeshComponent>();
        if(!object.has<MeshComponent>()){
            console_log_fatal("COULD NOT FIND MESH COMPONENT!!!");
            return;
        }
        if(component == nullptr){
            console_log_fatal("MeshComponent was invalid that was attempted to get in vulkan_renderer!!!");
            return;
        }
        
        Mesh mesh_data = component->MeshMetadata;

        auto& vb = mesh_data.get_vertices();
        auto ib = mesh_data.get_indices();


        vb->bind(current_cmd_buffer);

        if(ib != nullptr || ib->has_indices()){
            ib->bind(current_cmd_buffer);
            ib->draw(current_cmd_buffer);
        }
        else{
            vb->draw(current_cmd_buffer);
        }

        // if(ib != nullptr){
        //     ib->bind(current_cmd_buffer);
 
        //     if(ib->has_indices()){
        //         ib->draw(current_cmd_buffer);
        //     }
        //     else{
        //         vb->draw(current_cmd_buffer);
        //     }
        // }
        // else{
        //     vb->draw(current_cmd_buffer);
        // }
        
        
    }

    VkCommandBuffer vk_renderer::get_current_command_buffer(){
        return g_command_buffers[g_current_frame_index];
    }

    VkCommandBuffer vk_renderer::current_command_buffer(){
        return g_command_buffers[g_current_frame_index];
    }

    VkFramebuffer vk_renderer::current_frame_buffer(){
        return application_instance::get_window().get_current_swapchain()->get_framebuffer(g_current_frame_index);
    }

    uint32_t vk_renderer::get_current_command_buffer_index(){
        return g_current_frame_index;
    }
};