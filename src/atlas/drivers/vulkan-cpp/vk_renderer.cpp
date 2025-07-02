#include <drivers/vulkan-cpp/vk_renderer.hpp>
#include <array>
#include <drivers/vulkan-cpp/helper_functions.hpp>
#include <drivers/vulkan-cpp/vk_context.hpp>
#include <core/application.hpp>

#include <core/system_framework/system_registry.hpp>
#include <core/scene/world.hpp>
#include <drivers/vulkan-cpp/vk_types.hpp>

#include <future>

namespace atlas::vk {


    vk_renderer::vk_renderer(const vk_swapchain& p_swapchain, const std::string& p_tag){
        console_log_manager::create_new_logger(p_tag);
        m_main_swapchain = p_swapchain;
        m_image_count = p_swapchain.image_size();

        std::array<shader_info, 2> shader_sources = {
            shader_info{"experimental-shaders/test.vert", shader_stage::vertex},
            shader_info{"experimental-shaders/test.frag", shader_stage::fragment}
        }; 

        m_shader_group = vk_shader_group(shader_sources);

        std::array<vertex_attribute_entry, 4> attribute_entries = {
            vertex_attribute_entry{ .location = 0, .format = format::rgb32_sfloat, .stride = offsetof(vk::vertex_input, position) },
            vertex_attribute_entry{ .location = 1, .format = format::rgb32_sfloat, .stride = offsetof(vk::vertex_input, color) },
            vertex_attribute_entry{ .location = 2, .format = format::rgb32_sfloat, .stride = offsetof(vk::vertex_input, normals) },
            vertex_attribute_entry{ .location = 3, .format = format::rg32_sfloat, .stride = offsetof(vk::vertex_input, uv) }
        };

        std::array<vertex_attribute, 1> attribute = {
            vertex_attribute{ // layout (set = 0, binding = 0)
                .binding = 0,
                .entries = attribute_entries,
                .stride = sizeof(vk::vertex_input),
                .input_rate = input_rate::vertex
            }
        };

        m_shader_group.vertex_attributes(attribute);

        m_global_uniforms = std::vector<vk_uniform_buffer>(p_swapchain.image_size());

        for(size_t i = 0; i < m_global_uniforms.size(); i++) {
            m_global_uniforms[i] = vk_uniform_buffer(sizeof(camera_ubo));
        }


        // Descriptor Set 0
        std::vector<descriptor_binding_entry> entries = {
            descriptor_binding_entry{ // specifies "layout (set = 0, binding = 0) uniform GlobalUbo"
                .type = vk::buffer::uniform,
                .binding_point = {
                    .binding = 0,
                    .stage = shader_stage::vertex
                },
                .descriptor_count = 1,
            }
        };

        descriptor_set_layout set0_descriptor_layout = {
            .allocate_count = m_image_count,            // the count how many descriptor set layout able to be allocated
            .max_sets = m_image_count,                  // max of descriptor sets able to allocate
            .size_bytes = sizeof(camera_ubo),           // size of bytes of the uniforms utilized by this descriptor sets
            .entry = entries                            // specifies pool sizes and descriptor layout
        };

        m_global_descriptor = descriptor_set(0, set0_descriptor_layout);

        m_geometry_descriptor_layout = {
            m_global_descriptor.get_layout(),
        };

        /*
            Pipeline Layout
            [Set 0] [Set 1] [Set 2]    [Set 3]
               |      |       |          |
               v      v       v          v
            Camera   Material Textures Lights
            UBO      UBO     UBO     UBO
        */
        // m_main_pipeline = vk_pipeline(m_main_swapchain.swapchain_renderpass(), m_shader_group, set_layouts);

        m_global_descriptor.update(m_global_uniforms);

        vk_context::submit_resource_free([this](){
            m_shader_group.destroy();
            m_main_pipeline.destroy();
            // global uniforms camera data
            m_global_descriptor.destroy();

            for(size_t i = 0; i < m_global_uniforms.size(); i++) {
                m_global_uniforms[i].destroy();
            }

            for(auto[key, value] : m_cached_meshes) {
                console_log_fatal("Entity {} Destroyed in vk_renderer!!!", key);
                // To ensure that the mesh we are destroying are valid
                if(value.loaded()) {
                    value.destroy();
                }
            }

            for(auto[key, value] : m_geometry_descriptor) {
                value.destroy();
            }
        });
    }

    void vk_renderer::background_color(const std::array<float, 4>& p_color) {
        m_color = {{p_color.at(0), p_color.at(1), p_color.at(2), p_color.at(3)}};
    }

    void vk_renderer::start_frame(const vk_command_buffer& p_current, [[maybe_unused]] const vk::vk_swapchain& p_swapchain_handler) {
        // m_main_swapchain = p_swapchain_handler; // ?? This is here to do some testing with swapchain validation
        m_current_frame = application::current_frame();

        std::array<VkClearValue, 2> clear_values = {};

        clear_values[0].color = m_color;
        clear_values[1].depthStencil = {1.f, 0};
        window_settings settings = m_main_swapchain.settings();



        ref<world_scope> current_world = system_registry::get_world("Editor World");
        ref<scene_scope> current_scene = current_world->get_scene("LevelScene");

        flecs::query<> query_camera = current_scene->query_builder<camera>().build();

        query_camera.each([&](const flecs::entity& p_entity){
            const camera* camera_component = p_entity.get<camera>();
            if(camera_component->IsMainCamera) {
                m_camera = *p_entity.get<camera>();
            }
        });

        //! TODO: This will need to be changed.
        //! @brief THis is used to initialize our meshes but also before we begin recording the command buffers, we update the descriptor sets as well
        if(m_begin_initialize) {

            std::vector<descriptor_binding_entry> material_set1_entries = {
                descriptor_binding_entry{ // entry for layout (set = 1, binding = 0)
                    .type = vk::buffer::uniform,
                    .binding_point = {
                        .binding = 0,
                        .stage = shader_stage::vertex
                    },
                    .descriptor_count = 1
                },
                descriptor_binding_entry{ // entry for layout (set = 1, binding = 1)
                    .type = vk::buffer::combined_image_sampler,
                    .binding_point = {
                        .binding = 1,
                        .stage = shader_stage::fragment
                    },
                    .descriptor_count = 1
                },
                // descriptor_binding_entry{ // entry for layout (set = 1, binding = 2)
                //     .type = vk::buffer::combined_image_sampler,
                //     .binding_point = {
                //         .binding = 1,
                //         .stage = shader_stage::fragment
                //     },
                //     .descriptor_count = 1
                // }
            };

            descriptor_set_layout material_layout = {
                .allocate_count = m_image_count,            // the count how many descriptor set layout able to be allocated
                .max_sets = m_image_count,                  // max of descriptor sets able to allocate
                .size_bytes = sizeof(material_uniform),           // size of bytes of the uniforms utilized by this descriptor sets
                .entry = material_set1_entries                            // specifies pool sizes and descriptor layout
            };

            flecs::query<> caching = current_scene->query_builder<material>().build();

            caching.each([this, material_layout](flecs::entity p_entity){
                std::string name = std::string(p_entity.name().c_str());
                const material* target = p_entity.get<material>();

                //! @brief My attempt at loading meshes asynchronously and seeing that the mesh gets loaded and rendering already-loaded mesh.
                // std::future<mesh> launch_mesh_loading = std::async(std::launch::async, [target](){ return mesh(target->model_path); } );
                // mesh new_mesh = launch_mesh_loading.get();
                mesh new_mesh(std::filesystem::path(target->model_path));
                console_log_error("loaded = {}", new_mesh.loaded());
                if(new_mesh.loaded()) {
                    m_cached_meshes.emplace(name, new_mesh);

                    m_cached_meshes[name].initialize_uniforms(sizeof(material_uniform));
                    m_cached_meshes[name].add_texture(target->texture_path);

                    m_geometry_descriptor[name] = descriptor_set(1, material_layout);

                    // Apply mesh-specific uniforms to the mesh-specific descriptor (model matrix, color, etc)
                    std::array<vk_uniform_buffer, 1> material_uniforms = {
                        m_cached_meshes[name].material_ubo()
                    };
                    m_geometry_descriptor[name].update(material_uniforms, m_cached_meshes[name].read_textures());

                    m_geometry_descriptor_layout.push_back(m_geometry_descriptor[name].get_layout());
                }
            });

            console_log_trace("m_geometry_descriptor_layout.size() = {}", m_geometry_descriptor_layout.size());

            m_main_pipeline = vk_pipeline(m_main_swapchain.swapchain_renderpass(), m_shader_group, m_geometry_descriptor_layout);
            m_begin_initialize = false; 
        }

        VkRenderPassBeginInfo renderpass_begin_info = {
            .sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO,
			.pNext = nullptr,
			.renderPass = m_main_swapchain.swapchain_renderpass(),
			.renderArea = {
				.offset = {
					.x = 0,
					.y = 0
				},
				.extent = {
					.width = settings.width,
					.height = settings.height
				},
			},
			.clearValueCount = static_cast<uint32_t>(clear_values.size()),
			.pClearValues = clear_values.data()
        };

        m_current_command_buffer = p_current;
        m_current_command_buffer.begin(VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT);

        VkViewport viewport = {
            .x = 0.0f,
			.y = 0.0f,
			.width = static_cast<float>(settings.width),
			.height = static_cast<float>(settings.height),
			.minDepth = 0.0f,
			.maxDepth = 1.0f,
        };

        vkCmdSetViewport(m_current_command_buffer, 0, 1, &viewport);

		VkRect2D scissor = {
			.offset = { 0, 0 },
			.extent = {settings.width, settings.height},
		};

		vkCmdSetScissor(m_current_command_buffer, 0, 1, &scissor);

        renderpass_begin_info.framebuffer = m_main_swapchain.active_framebuffer(m_current_frame);

		vkCmdBeginRenderPass(m_current_command_buffer,&renderpass_begin_info,VK_SUBPASS_CONTENTS_INLINE);

    }

    void vk_renderer::post_frame() {
        // camera uniforms

        camera_ubo ubo{};
        ubo.View = m_camera.get_view();
        ubo.Projection = m_camera.get_projection();

        // For now, using this. Will need to remove this before vulkan integration merging into dev
        // This is for testing and to hopefully have a global_ubo for globalized uniforms
        global_ubo global_frame_ubo = {
            .mvp = ubo.Projection * ubo.View
        };

        // glm::mat4 mvp = ubo.Projection * ubo.View * ubo.Model;
        m_global_uniforms[m_current_frame].update(&global_frame_ubo);
        
        ref<world_scope> current_world = system_registry::get_world("Editor World");
        ref<scene_scope> current_scene = current_world->get_scene("LevelScene");

        //! TODO: Replace rendertarget3d with a material component
        // Should rmeove glm::mat4 from material-specific components
        flecs::query<> query_targets = current_scene->query_builder<material>().build();
        m_main_pipeline.bind(m_current_command_buffer);
        query_targets.each([this](flecs::entity p_entity){
            // const material* target = p_entity.get<material>();
            //! @brief This is going to be removed, because to prepare for the offloading of the rendering, that'll get done in preparation of that frame.
            //! @brief Meaning utilizing
            //! TODO: Probably have a way of handling reloading when a mesh 3d model filepath changes; this should be done when preparing the frame not as its rendering
            // if(target->texture_reload) {}

            const transform* transform_component = p_entity.get<transform>();
            const material* material_component = p_entity.get<material>();
            m_model = glm::mat4(1.f);
            m_model = glm::translate(m_model, transform_component->Position);
            m_model = glm::scale(m_model, transform_component->Scale);
            glm::mat4 rotation_mat4 = glm::mat4(glm::quat(transform_component->Rotation));

            m_model *= rotation_mat4;

            
            // Mesh used for viking_room - replaced with std::map equivalent
            material_uniform mesh_material_ubo = {
                .model = m_model,
                .color = material_component->color
            };

            std::string entity_name = std::string(p_entity.name().c_str());
            if(m_cached_meshes[entity_name].loaded()) {
                m_cached_meshes[entity_name].update_uniform(mesh_material_ubo);

                // Bind global camera data here
                m_global_descriptor.bind(m_current_command_buffer, m_current_frame, m_main_pipeline.get_layout());

                // Bind mesh-entity specific properties here before initial draw call
                m_geometry_descriptor[entity_name].bind(m_current_command_buffer, m_current_frame, m_main_pipeline.get_layout());
                m_cached_meshes[entity_name].draw(m_current_command_buffer);
            }
            
        });
        
        vkCmdEndRenderPass(m_current_command_buffer);
        m_current_command_buffer.end();

        m_main_swapchain.submit(m_current_command_buffer);
    }
};