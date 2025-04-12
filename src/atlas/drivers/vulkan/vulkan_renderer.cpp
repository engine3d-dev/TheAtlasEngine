#include <atomic>
#include <core/application.hpp>
#include <core/engine_logger.hpp>
#include <core/update_handlers/sync_update.hpp>
#include <cstring>
#include <drivers/vulkan/helper_functions.hpp>
#include <drivers/vulkan/shaders/vulkan_shader.hpp>
#include <drivers/vulkan/vulkan_context.hpp>
#include <drivers/vulkan/vulkan_renderer.hpp>
#include <drivers/vulkan/vulkan_swapchain.hpp>
#include <scene/scene_object.hpp>
#include <vector>
#include <vulkan/vulkan_core.h>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <core/image/stb_image.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <vector>

#include <drivers/ui/imgui_backend.hpp>

#include <core/event/event.hpp>

namespace atlas::vk {

    /**
     * @name uniform_buffer
     * @note TODO: Should probably be its own class that also enables to
     * read/write to it to the shaders
     */
    struct UniformBuffer {
        VkBuffer BufferHanlder;
        VkDeviceMemory BufferMemory;
        void* uniform_bufferMappedData;
    };

    std::atomic<uint32_t> g_current_frame_index = -1;

    //! @note Shader-definitions
    static ref<shader> g_shader = nullptr;
    //! @note Since descriptor sets aren't working, we are creating another
    //! shader just to set the colors lol

    struct CameraUbo {
        glm::mat4 Projection{ 1.f };
        glm::mat4 View{ 1.f };
        glm::mat4 Model{ 1.f };
        glm::vec3 LightPosition{ 1.0, -3.0, -1.0 };
        glm::vec4 Color{ 1.f };
        glm::vec2 MousePosition{ 0.f };
    };

    struct PointLighTest {
        glm::vec3 Position;
        glm::vec4 Ambient;
        glm::vec3 Color;
    };

    std::array<VkCommandBuffer, 3> g_command_buffers;

    vk_renderer::vk_renderer(const std::string& p_tag) {
        g_current_frame_index = 0;
        m_is_frame_started = false;
        if (p_tag.empty()) {
        }
        m_scene_object_queue = std::vector<ref<scene_object>>();

        // Initializing Uniform Buffers
        // Createuniform_buffers();

        // initializing descriptors
        // CreateDescriptorSetLayout();

        initialize_pipeline();
    }

    vk_renderer::~vk_renderer() {
        vkDestroyPipelineLayout(m_driver, m_pipeline_layout, nullptr);
    }

    void vk_renderer::initialize_pipeline() {
        auto max_frames_in_flight = vk_swapchain::MaxFramesInFlight;
        m_driver = vk_context::get_current_driver();

        m_global_pool = descriptor_pool::builder()
                          .setMaxSets(max_frames_in_flight)
                          .addPoolSize(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
                                       max_frames_in_flight)
                          .addPoolSize(VK_DESCRIPTOR_TYPE_STORAGE_BUFFER,
                                       max_frames_in_flight)
                          .build();

        m_global_set_layout = descriptor_set_layout::builder()
                                .addBinding(0,
                                            VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
                                            VK_SHADER_STAGE_VERTEX_BIT)
                                .build();

        m_global_descriptor_set =
          std::vector<VkDescriptorSet>(max_frames_in_flight);

        m_global_ubo_list.resize(max_frames_in_flight);

        for (size_t i = 0; i < m_global_ubo_list.size(); i++) {
            m_global_ubo_list[i] =
              BufferTutorial(sizeof(CameraUbo),
                             1,
                             VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
                             VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);
            m_global_ubo_list[i].map();
        }

        for (size_t i = 0; i < m_global_descriptor_set.size(); i++) {
            auto buffer_info = m_global_ubo_list[i].descriptor_info();

            descriptor_writer(*m_global_set_layout, *m_global_pool)
              .writeBuffer(0, &buffer_info)
              .build(m_global_descriptor_set[i]);
        }

        //! @note Setting up push constants
        //! @note VkPipelineLayoutCreateInfo does not require a push constant to
        //! be defined in it's configuration
        //! @note It can just be set to its default vulkan has its configuration
        g_current_frame_index = 0;
        m_is_frame_started = false;

        //! @note Setting up our pipeline.
        auto pipeline_config = vk::vk_shader::shader_configuration(
          application::get_window().get_width(),
          application::get_window().get_height());

        //! @note Initialize Push constant range

        VkPushConstantRange push_const_range = { .stageFlags =
                                                   VK_SHADER_STAGE_VERTEX_BIT |
                                                   VK_SHADER_STAGE_FRAGMENT_BIT,
                                                 .offset = 0,
                                                 .size = sizeof(CameraUbo) };

        //! @note We are setting our descriptors to work with layout(set = 0,
        //! binding = 0)
        // std::vector<VkDescriptorSetLayout> setLayouts;
        // setLayouts.push_back(m_global_set_layout->get_descriptor_set_layout());

        //! @note First initializing pipeline layout create info
        //! @note TODO: Handle if a pipeline layout has descriptors or no
        /*

            Pipeline API would look like the following

            Pipeline pipeline_a = Pipeline(descriptors: false, descriptors_count
           = 0, descriptors: std::span<DescriptorSet>);

        */

        std::vector<VkDescriptorSetLayout> descriptor_set_layouts = {
            m_global_set_layout->get_descriptor_set_layout()
        };

        VkPipelineLayoutCreateInfo pipeline_layout_create_info = {
            .sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
            .pNext = nullptr,
            .flags = 0,
            // .setLayoutCount = 0,
            // .pSetLayouts = nullptr,
            .setLayoutCount =
              static_cast<uint32_t>(descriptor_set_layouts.size()),
            .pSetLayouts = descriptor_set_layouts.data(),
            // .setLayoutCount = 1,
            // .pSetLayouts = &g_DescriptorSetLayout,
            // .setLayoutCount = 1,
            // .pSetLayouts = &info.Layout,
            .pushConstantRangeCount = 1,
            .pPushConstantRanges = &push_const_range
        };

        vk::vk_check(vkCreatePipelineLayout(m_driver,
                                            &pipeline_layout_create_info,
                                            nullptr,
                                            &m_pipeline_layout),
                     "vkCreatePipelineLayout",
                     __FILE__,
                     __LINE__,
                     __FUNCTION__);

        //! @note We are setting our shader pipeline to utilize our current
        //! window's swapchain
        //! @note a TODO is to utilize different render passes utiization for
        //! shader pipelines, potentially.
        pipeline_config.PipelineRenderPass =
          application::get_window().get_current_swapchain()->get_renderpass();
        pipeline_config.PipelineLayout = m_pipeline_layout;

        // m_Shader = shader::create("simple_shader/simple_shader.vert.spv",
        // "simple_shader/simple_shader.frag.spv", pipeline_config);
        g_shader =
          shader::create("shaders/mouse_picking_shaders/simple_shader.vert.spv",
                         "shaders/mouse_picking_shaders/simple_shader.frag.spv",
                         pipeline_config);
        // g_ColorShader =
        // shader::create("shader_ubo_tutorial/simple_shader.vert.spv",
        // "shader_ubo_tutorial/simple_shader.frag.spv", pipeline_config);

        // if()
        console_log_error("NOT AN ERROR: Shader Loaded Successfully!");

        //! @note Initializing Command buffers.
        // g_command_buffers.resize(
        //   application::get_window().get_current_swapchain()->get_images_size());
        for (uint32_t i = 0; i < g_command_buffers.size(); i++) {
            g_command_buffers[i] = {};
        }

        VkCommandPoolCreateInfo pool_create_info = {
            .sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
            .pNext = nullptr,
            .flags = VK_COMMAND_POOL_CREATE_TRANSIENT_BIT |
                     VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT,
            .queueFamilyIndex =
              vk::vk_context::get_current_selected_physical_driver()
                .get_queue_indices()
                .Graphics
        };

        vk::vk_check(vkCreateCommandPool(
                       m_driver, &pool_create_info, nullptr, &m_command_pool),
                     "vkCreateCommandPool",
                     __FILE__,
                     __LINE__,
                     __FUNCTION__);

        console_log_info("RENDERER COMMAND BUFFERS SIZE === {}",
                         g_command_buffers.size());
        //! @note Allocating our command buffers.
        VkCommandBufferAllocateInfo cmd_buffer_alloc_info = {
            .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
            .commandPool = m_command_pool,
            .level = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
            .commandBufferCount =
              static_cast<uint32_t>(g_command_buffers.size()),
        };

        vk::vk_check(vkAllocateCommandBuffers(m_driver,
                                              &cmd_buffer_alloc_info,
                                              g_command_buffers.data()),
                     "vkAllocateCommandBuffers",
                     __FILE__,
                     __LINE__,
                     __FUNCTION__);

        console_log_info("CommandBuffers Size === {}",
                         g_command_buffers.size());
    }

    void vk_renderer::begin_frame() {

        if (swapchain::is_rebuild()) {
            console_log_fatal("ARE YOU CALED AFTER RESIZE?!?!");
            initialize_pipeline();
            swapchain::reset_build();
        }

        //! @note Note to Self: This is just a hack on how to get "resizing" to
        //! work
        //! @note I will need to do this better as there are problems with this.
        //! @note Currently the state is if the window is resized by
        //! glfwSetFramebufferSizeCallback in vulkan_window.cpp, we also check
        //! here for the state of our swapchain.
        //! @note If our swapchain's state is either VK_ERROR_OUT_OF_DATE_KHR or
        //! VK_SUBOPTIMAL_KHR, then the Acquired function returns -3.
        //! @note -3 was just to indicate the swapchain's state as been modified
        //! and needs to modify the pipeline to work with the new swapchain.
        //! @note Another TODO: Make abstract the pipeline's so that we do not
        //! have to modify this directly.
        //! @note If this isn't already done, but the vk::submit function should
        //! handle the way we directly handle these tasks... potentially?
        g_current_frame_index = application::get_window()
                                  .get_current_swapchain()
                                  ->acquire_next_image();
        if (swapchain::is_rebuild()) {
            console_log_fatal_tagged(
              "vulkan", "How often are you being called to be initialized?");
            // renderer::Initialize();
            initialize_pipeline();
        }

        VkCommandBufferBeginInfo cmd_buffer_begin_info = {
            .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
        };

        auto cmd_buffer = get_current_command_buffer();

        vk::vk_check(vkBeginCommandBuffer(cmd_buffer, &cmd_buffer_begin_info),
                     "vkBeginCommandBuffer",
                     __FILE__,
                     __LINE__,
                     __FUNCTION__);
        VkRenderPassBeginInfo rp_begin_info = {
            .sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO,
            .renderPass = application::get_window()
                            .get_current_swapchain()
                            ->get_renderpass(),
            .framebuffer =
              application::get_window()
                .get_current_swapchain()
                ->get_framebuffer(
                  g_current_frame_index), // Specifying which framebuffer to
                                          // render pass to.
            .renderArea = { .offset = { 0, 0 },
                            .extent = { application::get_window().get_width(),
                                        application::get_window()
                                          .get_height() } }
        };

        std::array<VkClearValue, 2> clear_values;
        clear_values[0].color = { { 0.1f, 0.1f, 0.1f, 1.0f } };
        clear_values[1].depthStencil = { 1.0f, 0 };

        rp_begin_info.clearValueCount =
          static_cast<uint32_t>(clear_values.size());
        rp_begin_info.pClearValues = clear_values.data();

        vkCmdBeginRenderPass(
          cmd_buffer, &rp_begin_info, VK_SUBPASS_CONTENTS_INLINE);
        m_is_frame_started = true;

        VkViewport viewport = {
            .x = 0.0f,
            .y = 0.0f,
            .width = static_cast<float>(application::get_window()
                                          .get_current_swapchain()
                                          ->get_extent()
                                          .width),
            .height = static_cast<float>(application::get_window()
                                           .get_current_swapchain()
                                           ->get_extent()
                                           .height),
            .maxDepth = 1.0f,
        };

        VkRect2D scissor = {
            .offset = { 0, 0 },
            .extent =
              application::get_window().get_current_swapchain()->get_extent()
        };

        g_command_buffers[g_current_frame_index] = cmd_buffer;

        vkCmdSetViewport(
          g_command_buffers[g_current_frame_index], 0, 1, &viewport);
        vkCmdSetScissor(
          g_command_buffers[g_current_frame_index], 0, 1, &scissor);

        ImGuiBackend::Begin();

        //! @note Pre-computing our camera properties and applying it to our
        //! scene objects

        //! TODO: Fix: The bug is this only works when dealing with a single
        //! scene
        flecs::world* world_object =
          system_registry::get_world().get_registry();
        flecs::query<> queried_render_targets =
          world_object->query_builder<camera>().build();

        //! @note The idea behind this is we pre-determine this at the beginning
        //! of the frame to getting our camera
        //! @note Then once we get our camera properties that then gets applied
        //! to our object's that uses that camera
        queried_render_targets.each([&](flecs::entity p_entity_id) {
            if (p_entity_id.has<camera>()) {
                if (p_entity_id.get<camera>()->IsMainCamera) {
                    m_current_camera_component = *p_entity_id.get<camera>();
                }
            }
        });
    }

    void vk_renderer::end_frame() {
        auto current_cmd_buffer = get_current_command_buffer();

        //! @note Flushing out scene objects

        // for()

        //! TODO: Fix: The bug is this only works when dealing with a single
        //! scene
        flecs::world* world_object =
          system_registry::get_world().get_registry();
        flecs::query<> queried_render_targets =
          world_object->query_builder<RenderTarget3D>().build();

        //! @note The idea behind this is we pre-determine this at the beginning
        //! of the frame to getting our camera
        //! @note Then once we get our camera properties that then gets applied
        //! to our object's that uses that camera
        queried_render_targets.each([&](flecs::entity p_entity_id) {
            const Transform* transform_component = p_entity_id.get<Transform>();
            vkCmdBindPipeline(current_cmd_buffer,
                              VK_PIPELINE_BIND_POINT_GRAPHICS,
                              g_shader->get_graphics_pipeline());

            glm::mat4 model = p_entity_id.get<RenderTarget3D>()->Model;
            model = glm::translate(model, transform_component->Position);
            model = glm::scale(model, transform_component->Scale);
            auto rotation_mat4 =
              glm::mat4(glm::quat(transform_component->Rotation));
            model *= rotation_mat4;

            /*

            Camera
            For the Projection and View matrices are going to be handled by the
            camera component These get determined pre-frame. Within begin_frame

            RenderTarget
            - Gets determined pre-frame and offloaded to GPU at the end of frame

            */
            CameraUbo push_const_data = {
                // .Projection = camera_component->get_projection(),
                // .View = camera_component->get_view(),
                .Projection = m_current_camera_component.get_projection(),
                .View = m_current_camera_component.get_view(),
                .Model = model,
                // .LightPosition = -camera_component->Front,
                .LightPosition = -m_current_camera_component.get_front(),
                // .LightPosition = point_light.Position,
                .Color = transform_component->Color,
                // .MousePosition = event::cursor_position()
                .MousePosition = { event::cursor_position().x /
                                     (float)application::get_window()
                                       .get_width(),
                                   event::cursor_position().y /
                                     (float)application::get_window()
                                       .get_height() }
            };

            vkCmdPushConstants(current_cmd_buffer,
                               m_pipeline_layout,
                               VK_SHADER_STAGE_VERTEX_BIT |
                                 VK_SHADER_STAGE_FRAGMENT_BIT,
                               0,
                               sizeof(CameraUbo),
                               &push_const_data);

            if (!p_entity_id.has<RenderTarget3D>()) {
                console_log_fatal("COULD NOT FIND MESH COMPONENT!!!");
                return;
            }

            auto* component = p_entity_id.get<RenderTarget3D>();

            Mesh mesh_data = component->MeshMetaData;

            auto& vb = mesh_data.get_vertex_buffer();
            auto ib = mesh_data.get_index_buffer();

            vb->bind(current_cmd_buffer);

            if (ib != nullptr || ib->has_indices()) {
                ib->bind(current_cmd_buffer);
                ib->draw(current_cmd_buffer);
            }
            else {
                vb->draw(current_cmd_buffer);
            }
        });

        ImGuiBackend::End();
        vkCmdEndRenderPass(current_cmd_buffer);
        vk::vk_check(vkEndCommandBuffer(current_cmd_buffer),
                     "vkEndCommandBuffer",
                     __FILE__,
                     __LINE__,
                     __FUNCTION__);

        application::get_window()
          .get_current_swapchain()
          ->submit_command_buffer(&current_cmd_buffer);
        m_is_frame_started = false;
    }

    VkCommandBuffer vk_renderer::get_current_command_buffer() {
        return g_command_buffers[g_current_frame_index];
    }

    VkFramebuffer vk_renderer::current_frame_buffer() {
        return application::get_window()
          .get_current_swapchain()
          ->get_framebuffer(g_current_frame_index);
    }

    uint32_t vk_renderer::get_current_command_buffer_index() {
        return g_current_frame_index;
    }
};