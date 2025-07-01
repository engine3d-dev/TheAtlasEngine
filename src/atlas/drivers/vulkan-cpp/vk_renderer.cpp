#include <drivers/vulkan-cpp/vk_renderer.hpp>
#include <core/engine_logger.hpp>
#include <array>
#include <drivers/vulkan-cpp/helper_functions.hpp>
#include <drivers/vulkan-cpp/vk_context.hpp>
#include <core/application.hpp>

#include <glm/glm.hpp>
#include <type_traits>

#include <core/system_framework/system_registry.hpp>
#include <core/scene/world.hpp>

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
            vertex_attribute{
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


        // Descriptor Set 1
        // Something to Note: beause we already have the binding available here
        // What we can do, is also reuse these binding points to also set the .dstBinding parameter set in the VkWriteDescriptorSet
        std::vector<descriptor_binding_entry> set1_entries = {
            descriptor_binding_entry{ // entry for layout (set = 1, binding = 0)
                .type = vk::buffer::uniform,
                .binding_point = {
                    .binding = 0,
                    .stage = shader_stage::vertex
                },
                .descriptor_count = 1
            },
            descriptor_binding_entry{ // entry for layout (set = 1, binding = 1)
                .type = vk::buffer::image_sampler,
                .binding_point = {
                    .binding = 1,
                    .stage = shader_stage::fragment
                },
                .descriptor_count = 1
            },
        };

        // descriptor_set_layout material_descriptor_layout = {
        //     .allocate_count = m_image_count,            // the count how many descriptor set layout able to be allocated
        //     .max_sets = m_image_count,                  // max of descriptor sets able to allocate
        //     .size_bytes = sizeof(material),             // size of bytes of the uniforms utilized by this descriptor sets
        //     .entry = set1_entries                       // specifies pool sizes and descriptor layout
        // };

        m_material_descriptor_layout  = {
            .allocate_count = m_image_count,            // the count how many descriptor set layout able to be allocated
            .max_sets = m_image_count,                  // max of descriptor sets able to allocate
            .size_bytes = sizeof(material),             // size of bytes of the uniforms utilized by this descriptor sets
            .entry = set1_entries                       // specifies pool sizes and descriptor layout
        };

        //! @brief Just setting up a lookup table for specific shader resource data layout
        //! @brief In this case this is the data layout for materials-specifically
        // m_descriptor_table["material"] = material_descriptor_layout;
        // m_material_descriptor_layout = material_descriptor_layout;

        // m_descriptor_set1 = descriptor_set(1, m_material_descriptor_layout);
        // m_descriptor_set2 = descriptor_set(1, material_descriptor_layout);

        // m_mesh0_material_ubo = vk_uniform_buffer(sizeof(material));
        // m_mesh1_material_ubo = vk_uniform_buffer(sizeof(material));

        // std::vector<VkDescriptorSetLayout> set_layouts = {
        //     m_global_descriptor.get_layout(),
        //     // m_descriptor_set1.get_layout(),
        //     // m_descriptor_set2.get_layout()
        // };
        m_geometry_descriptor_layout = {
            m_global_descriptor.get_layout(),
        };

        // for(const auto&[key, value] : m_geometry_descriptor) {
        //     m_geometry_descriptor_layout.emplace_back(value.get_layout());
        // }
        /*
            Pipeline Layout
            [Set 0] [Set 1] [Set 2]    [Set 3]
               |      |       |          |
               v      v       v          v
            Camera   Material Textures Lights
            UBO      UBO     UBO     UBO
        */
        // m_main_pipeline = vk_pipeline(m_main_swapchain.swapchain_renderpass(), m_shader_group, set_layouts);

        /*
        m_mesh0 = mesh(std::filesystem::path("assets/models/viking_room.obj"));
        m_mesh0.add_texture(2, std::filesystem::path("assets/models/viking_room.png"));
        m_mesh0.add_texture(1, std::filesystem::path("assets/models/container_diffuse.png"));

        m_mesh1 = mesh(std::filesystem::path("assets/models/colored_cube.obj"));
        m_mesh1.add_texture(1, std::filesystem::path("assets/models/wood.png"));
        */

        m_global_descriptor.update(m_global_uniforms);

        // std::array<vk_uniform_buffer, 1> buffers = {
        //     m_mesh0_material_ubo
        // };

        // m_descriptor_set1.update(buffers, m_mesh0.read_textures());

        // std::array<vk_uniform_buffer, 1> buffers2 = {
        //     m_mesh1_material_ubo
        // };
        // m_descriptor_set2.update(buffers2, m_mesh1.read_textures());


        // Loading all meshes
        // ref<world_scope> current_world = system_registry::get_world("Editor World");
        // ref<scene_scope> current_scene = current_world->get_scene("LevelScene");

        // flecs::query<> caching = current_scene->query_builder<rendertarget3d>().build();

        // caching.each([&](flecs::entity p_entity){
        //     std::string name = std::string(p_entity.name().c_str());
        //     const rendertarget3d* target = p_entity.get<rendertarget3d>();
        //     m_cached_meshes[name] = mesh(std::filesystem::path(target->model_path));
        //     m_cached_meshes[name].add_texture(0, target->Filepath);
        // });




        vk_context::submit_resource_free([this](){
            m_shader_group.destroy();
            m_main_pipeline.destroy();
            // global uniforms camera data
            m_global_descriptor.destroy();

            // mesh 0
            // mesh 0 - descriptor set for material
            // mesh 0 descriptor set - update with vk_uniform_buffer to write material uniforms
            // m_descriptor_set1.destroy();

            // mesh 1
            // mesh 1 - descriptor set for material
            // mesh 1 descriptor set - update with vk_uniform_buffer to write material uniforms
            // m_descriptor_set2.destroy();
            // m_mesh0_material_ubo.destroy();
            // m_mesh1_material_ubo.destroy();

            for(size_t i = 0; i < m_global_uniforms.size(); i++) {
                m_global_uniforms[i].destroy();
            }

            // m_mesh0.destroy();
            // m_mesh1.destroy();

            for(auto[key, value] : m_cached_meshes) {
                console_log_fatal("Entity {} Destroyed in vk_renderer!!!", key);
                value.destroy();
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
            // Descriptor Set 0
            // std::vector<descriptor_binding_entry> entries = {
            //     descriptor_binding_entry{ // specifies "layout (set = 0, binding = 0) uniform GlobalUbo"
            //         .type = vk::buffer::uniform,
            //         .binding_point = {
            //             .binding = 0,
            //             .stage = shader_stage::vertex
            //         },
            //         .descriptor_count = 1,
            //     }
            // };

            // descriptor_set_layout set0_descriptor_layout = {
            //     .allocate_count = m_image_count,            // the count how many descriptor set layout able to be allocated
            //     .max_sets = m_image_count,                  // max of descriptor sets able to allocate
            //     .size_bytes = sizeof(camera_ubo),           // size of bytes of the uniforms utilized by this descriptor sets
            //     .entry = entries                            // specifies pool sizes and descriptor layout
            // };

            // m_descriptor_set0 = descriptor_set(0, set0_descriptor_layout);

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
                    .type = vk::buffer::image_sampler,
                    .binding_point = {
                        .binding = 1,
                        .stage = shader_stage::fragment
                    },
                    .descriptor_count = 1
                },
                // descriptor_binding_entry{ // entry for layout (set = 1, binding = 2)
                //     .type = vk::buffer::image_sampler,
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
                .size_bytes = sizeof(material),           // size of bytes of the uniforms utilized by this descriptor sets
                .entry = material_set1_entries                            // specifies pool sizes and descriptor layout
            };
            flecs::query<> caching = current_scene->query_builder<rendertarget3d>().build();

            caching.each([this, material_layout](flecs::entity p_entity){
                std::string name = std::string(p_entity.name().c_str());
                const rendertarget3d* target = p_entity.get<rendertarget3d>();

                m_cached_meshes.emplace(name, mesh(std::filesystem::path(target->model_path)));

                m_cached_meshes[name].initialize_uniforms(sizeof(material));
                m_cached_meshes[name].add_texture(target->Filepath);

                m_geometry_descriptor[name] = descriptor_set(1, material_layout);
                std::array<vk_uniform_buffer, 1> material_uniforms = {
                    m_cached_meshes[name].material_ubo()
                };
                m_geometry_descriptor[name].update(material_uniforms, m_cached_meshes[name].read_textures());

                m_geometry_descriptor_layout.push_back(m_geometry_descriptor[name].get_layout());
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


        // flecs::query<> query_targets = current_scene->query_builder<rendertarget3d>().build();

        // query_targets.each([this](flecs::entity p_entity){
        //     reload* reload_component = p_entity.get_mut<reload>();
            
        //     // Reloads model
        //     if(reload_component->on_reload) {
        //     }

        //     // Reloads texture
        //     if(reload_component->on_texture_reload) {
        //     }
        // });


    }

    void vk_renderer::post_frame() {
        // camera uniforms

        camera_ubo ubo{};
        ubo.View = m_camera.get_view();
        ubo.Projection = m_camera.get_projection();

        // ubo.Model = glm::inverse(ubo.Model);
        
        // ubo.View = glm::lookAt(glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        // ubo.Projection = glm::perspective(glm::radians(45.0f), (float)settings.width / (float) settings.height, 0.1f, 10.0f);
        // ubo.Projection[1][1] *= -1;

        // For now, using this. Will need to remove this before vulkan integration merging into dev
        // This is for testing and to hopefully have a global_ubo for globalized uniforms
        global_ubo global_frame_ubo = {
            .mvp = ubo.Projection * ubo.View
        };

        // glm::mat4 mvp = ubo.Projection * ubo.View * ubo.Model;
        m_global_uniforms[m_current_frame].update(&global_frame_ubo);

        // Model operation for mesh1
        // ubo.Model = glm::mat4(1.f);
        // ubo.Model = glm::translate(ubo.Model, glm::vec3(0.f));
        // ubo.Model = glm::scale(ubo.Model, glm::vec3(1.f, 1.f, 1.f));
        // ubo.Model = glm::rotate(ubo.Model, time * glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));

        
        ref<world_scope> current_world = system_registry::get_world("Editor World");
        ref<scene_scope> current_scene = current_world->get_scene("LevelScene");

        //! TODO: Replace rendertarget3d with a material component
        // Should rmeove glm::mat4 from material-specific components
        flecs::query<> query_targets = current_scene->query_builder<rendertarget3d>().build();
        m_main_pipeline.bind(m_current_command_buffer);
        query_targets.each([this](flecs::entity p_entity){
            const rendertarget3d* target = p_entity.get<rendertarget3d>();
            //! @brief This is going to be removed, because to prepare for the offloading of the rendering, that'll get done in preparation of that frame.
            //! @brief Meaning utilizing 
            if(target->is_texture_dirty) {
                
                
                    // When I get back, these are TODO's

                    // TODO's for meshes
                    // * Setting up rendertarget to load a .obj model
                    // * Setting up rendertarget3d to also load in one texture
                    // * Setting up parameter to indicate that these two load factors are dirty whenever they are changed. (for reloading)
                    // * Actually reload_rendertarget might actually be a component (for the time being)
                    //     * Reasons is you cannot update descriptors during recording command buffer state, as this becomes undefined behavior
                    //     * This is something I will address later, once vulkan integration gets into dev.
                    // * Ways how this is going to work is query_builder<rendertarget3d, reload> and then we are going to fetch all objects that are render targets
                    //     * Then through this query, we are going to reload during the begin_frame process to prepare the mesh for rendering.

                    //     Meaning in order to reload a mesh object, you need to do: m_sphere->add<atlas::rendertarget3d>();
                    //     And then to reload that mesh object once more, we do: m_sphere->set<atlas::reload>({ .on_reload = true });

                    //     Essentially if you want to think about this, its pretty much similar to sending a request to the renderer to reload these specific meshes before they get rendered 
                
            }
            const transform* transform_component = p_entity.get<transform>();
            m_model = glm::mat4(1.f);
            m_model = glm::translate(m_model, transform_component->Position);
            m_model = glm::scale(m_model, transform_component->Scale);
            glm::mat4 rotation_mat4 = glm::mat4(glm::quat(transform_component->Rotation));

            m_model *= rotation_mat4;

            
            // Mesh used for viking_room - replaced with std::map equivalent
            material mesh_material_ubo = {
                .model = m_model,
                // .color = {1.f, 1.f, 1.f, 1.0f}
                .color = transform_component->Color
            };

            std::string entity_name = std::string(p_entity.name().c_str());
            m_cached_meshes[entity_name].update_uniform(mesh_material_ubo);

            // Bind global camera data here
            m_global_descriptor.bind(m_current_command_buffer, m_current_frame, m_main_pipeline.get_layout());

            // Bind mesh-entity specific properties here before initial draw call
            m_geometry_descriptor[entity_name].bind(m_current_command_buffer, m_current_frame, m_main_pipeline.get_layout());
            m_cached_meshes[entity_name].draw(m_current_command_buffer);
            
        });
        

        // viking_room + viking_room_texture
        /*
        m_main_pipeline.bind(m_current_command_buffer);
        m_descriptor_set0.bind(m_current_command_buffer, m_current_frame, m_main_pipeline.get_layout());
        m_descriptor_set1.bind(m_current_command_buffer, m_current_frame, m_main_pipeline.get_layout());

        m_mesh0.draw(m_current_command_buffer);
        */




        // cube + wood texture
        /*
        ubo.Model = glm::mat4(1.f);
        ubo.Model = glm::translate(ubo.Model, glm::vec3(0.f));
        ubo.Model = glm::scale(ubo.Model, glm::vec3(0.5f, 0.5f,0.5f));
        // ubo.Model = glm::scale(ubo.Model, glm::vec3(0.5f, 0.5f, 0.5f));
        ubo.Model = glm::rotate(ubo.Model, time * glm::radians(-90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        ubo.Model = glm::inverse(ubo.Model);
        material mesh1_material_ubo = {
            .model = ubo.Model,
            .color = {1.f, 1.f, 1.f, 0.7}
        };
        
        m_mesh1_material_ubo.update(&mesh1_material_ubo);

        m_descriptor_set0.bind(m_current_command_buffer, m_current_frame, m_main_pipeline.get_layout());
        m_descriptor_set2.bind(m_current_command_buffer, m_current_frame, m_main_pipeline.get_layout());
        m_mesh1.draw(m_current_command_buffer);
        */


        vkCmdEndRenderPass(m_current_command_buffer);
        m_current_command_buffer.end();

        m_main_swapchain.submit(m_current_command_buffer);
    }
};