#include <core/system/registry.hpp>
#include <core/scene/world.hpp>
#include <drivers/vulkan-cpp/vk_renderer.hpp>

#include <array>
#include <drivers/vulkan-cpp/helper_functions.hpp>
#include <drivers/vulkan-cpp/vk_context.hpp>
#include <core/application.hpp>

#include <drivers/vulkan-cpp/vk_types.hpp>

namespace atlas::vk {

    vk_renderer::vk_renderer(const window_settings& p_settings,
                             uint32_t p_image_size,
                             const std::string& p_tag) {
        console_log_manager::create_new_logger(p_tag);
        m_device = vk_context::driver_context();
        m_physical = vk_context::physical_driver();
        m_window_extent = p_settings;

        m_image_count = p_image_size;
#ifdef USE_SHADERC
        std::array<::vk::shader_source, 2> shader_sources = {
            ::vk::shader_source{
              "experimental-shaders/test.vert",
              ::vk::shader_stage::vertex,
            },
            ::vk::shader_source{
              "experimental-shaders/test.frag",
              ::vk::shader_stage::fragment,
            }
        };
#else
        std::array<::vk::shader_source, 2> shader_sources = {
            ::vk::shader_source{
              "experimental-shaders/test.vert.spv",
              ::vk::shader_stage::vertex,
            },
            ::vk::shader_source{
              "experimental-shaders/test.frag.spv",
              ::vk::shader_stage::fragment,
            }
        };
#endif

        std::array<::vk::vertex_attribute_entry, 4> attribute_entries = {
            ::vk::vertex_attribute_entry{
              .location = 0,
              .format = ::vk::format::rgb32_sfloat,
              .stride = offsetof(vk::vertex_input, position),
            },
            ::vk::vertex_attribute_entry{
              .location = 1,
              .format = ::vk::format::rgb32_sfloat,
              .stride = offsetof(vk::vertex_input, color),
            },
            ::vk::vertex_attribute_entry{
              .location = 2,
              .format = ::vk::format::rgb32_sfloat,
              .stride = offsetof(vertex_input, normals),
            },
            ::vk::vertex_attribute_entry{
              .location = 3,
              .format = ::vk::format::rg32_sfloat,
              .stride = offsetof(vertex_input, uv),
            }
        };

        std::array<::vk::vertex_attribute, 1> attribute = {
            ::vk::vertex_attribute{
              // layout (set = 0, binding = 0)
              .binding = 0,
              .entries = attribute_entries,
              .stride = sizeof(vk::vertex_input),
              .input_rate = ::vk::input_rate::vertex,
            },
        };

        ::vk::shader_resource_info shader_info = {
            .sources = shader_sources,
        };
        m_shader_group = shader_resource_group(m_device, shader_info);
        m_shader_group.vertex_attributes(attribute);

        // Setting global descriptor set 0
        std::vector<::vk::descriptor_entry> set0_entries = {
			::vk::descriptor_entry{
				// specifies "layout (set = 0, binding = 0) uniform GlobalUbo"
				.type = ::vk::buffer::uniform,
				.binding_point = {
					.binding = 0,
					.stage = ::vk::shader_stage::vertex,
				},
				.descriptor_count = 1,
			},
		};

        // uint32_t image_count = image_count;
        ::vk::descriptor_layout set0_layout = {
            .slot = 0,
            .allocate_count = m_image_count,
            .max_sets = m_image_count,
            .size_bytes = sizeof(global_ubo),
            .entries = set0_entries,
        };
        m_global_descriptors = ::vk::descriptor_resource(m_device, set0_layout);

        ::vk::uniform_buffer_info geo_info = {
            .phsyical_memory_properties = m_physical.memory_properties(),
            .size_bytes = sizeof(global_ubo),
        };
        m_global_uniforms = ::vk::uniform_buffer(m_device, geo_info);

        std::array<::vk::write_buffer_descriptor, 1> set0_write_buffers = {
            ::vk::write_buffer_descriptor{
              .dst_binding = 0,
              .buffer = m_global_uniforms,
              .offset = 0,
              .range = m_global_uniforms.size_bytes(),
            }
        };
        m_global_descriptors.update(set0_write_buffers);

        m_sets_layouts = {
            m_global_descriptors.layout(),
        };

        ::vk::image_extent extent = {
            .width = 1,
            .height = 1,
        };
        m_white_texture =
          ::vk::texture(m_device, extent, m_physical.memory_properties());

        vk_context::submit_resource_free([this]() {
            m_white_texture.destroy();
            m_shader_group.destroy();
            m_global_descriptors.destroy();
            m_global_uniforms.destroy();
            for (auto& [key, value] : m_cached_meshes) {
                console_log_trace("Entity \"{}\" Destroyed in vk_renderer!!!",
                                  key);

                value.destroy();
            }
            for (auto& [key, descriptor_map] : m_mesh_descriptors) {
                for (auto& [descriptor_type, descriptor] : descriptor_map) {
                    descriptor.destroy();
                }
            }
            m_main_pipeline.destroy();
        });
    }

    void vk_renderer::background_color(const std::array<float, 4>& p_color) {
        m_color = {
            { p_color.at(0), p_color.at(1), p_color.at(2), p_color.at(3) }
        };
    }

    void vk_renderer::start_frame(const ::vk::command_buffer& p_current,
                                  const window_settings& p_settings,
                                  const VkRenderPass& p_renderpass,
                                  const VkFramebuffer& p_framebuffer,
                                  const glm::mat4& p_proj_view) {
        m_proj_view = p_proj_view;
        m_current_frame = application::current_frame();

        std::array<VkClearValue, 2> clear_values = {};

        clear_values[0].color = m_color;
        clear_values[1].depthStencil = { 1.f, 0 };
        m_window_extent = p_settings;

        if (m_begin_initialize) {
            // set 1 -- material uniforms
            ref<world_scope> current_world =
              system_registry::get_world("Editor World");
            ref<scene_scope> current_scene =
              current_world->get_scene("LevelScene");

            flecs::query<> caching =
              current_scene->query_builder<mesh_source>().build();

            caching.each([this](flecs::entity p_entity) {
                const mesh_source* target = p_entity.get<mesh_source>();
                mesh new_mesh(std::filesystem::path(target->model_path));
                new_mesh.initialize_uniforms(sizeof(material_uniform));
                new_mesh.add_texture(
                  std::filesystem::path(target->texture_path));

                if (new_mesh.loaded()) {
                    m_cached_meshes.emplace(p_entity.id(), new_mesh);

                    std::vector<::vk::descriptor_entry> set1_entries = {
						::vk::descriptor_entry{
							// specifies "layout (set = 1, binding = 0) uniform MaterialSource"
							.type = ::vk::buffer::uniform,
							.binding_point = {
								.binding = 0,
								.stage = ::vk::shader_stage::vertex,
							},
							.descriptor_count = 1,
						},
						::vk::descriptor_entry{
							// specifies "layout (set = 1, binding = 1) uniform sampler2D texture1"
							.type = ::vk::buffer::combined_image_sampler,
							.binding_point = {
								.binding = 1,
								.stage = ::vk::shader_stage::fragment,
							},
							.descriptor_count = 1,
						},
					};

                    ::vk::descriptor_layout set1_layout = {
                        .slot = 1,
                        .allocate_count = m_image_count,
                        .max_sets = m_image_count,
                        .size_bytes = sizeof(material_uniform),
                        .entries = set1_entries,
                    };

                    m_mesh_descriptors[p_entity.id()].emplace(
                      "materials",
                      ::vk::descriptor_resource(m_device, set1_layout));

                    // layout(set=  1, binding = 0)
                    std::vector<::vk::write_buffer_descriptor>
                      material_uniforms = {
                          ::vk::write_buffer_descriptor{
                            .dst_binding = 0,
                            .buffer =
                              m_cached_meshes[p_entity.id()].material_ubo(),
                            .offset = 0,
                            .range = m_cached_meshes[p_entity.id()]
                                       .material_ubo()
                                       .size_bytes(),
                          },
                      };
                    // layout(set = 1, binding = 1)

                    // we provide a default white texture if no texture has been
                    // provided to this mesh
                    ::vk::sample_image default_write_image =
                      m_white_texture.image();
                    if (m_cached_meshes[p_entity.id()].texture_loaded()) {
                        default_write_image =
                          m_cached_meshes[p_entity.id()].image();
                    }

                    // creating our image descriptor to write to the shader
                    std::vector<::vk::write_image_descriptor>
                      material_textures = {
                          ::vk::write_image_descriptor{
                            .dst_binding = 1,
                            .view = default_write_image.image_view(),
                            .sampler = default_write_image.sampler(),
                          },
                      };

                    m_mesh_descriptors[p_entity.id()]["materials"].update(
                      material_uniforms, material_textures);

                    m_sets_layouts.push_back(
                      m_mesh_descriptors[p_entity.id()]["materials"].layout());
                }
            });

            std::vector<::vk::shader_handle> modules = m_shader_group.handles();

            ::vk::pipeline_settings pipeline_configuration = {
                .renderpass = p_renderpass,
                .shader_modules = modules,
                .vertex_attributes = m_shader_group.vertex_attributes(),
                .vertex_bind_attributes =
                  m_shader_group.vertex_bind_attributes(),
                .descriptor_layouts = m_sets_layouts
            };
            m_main_pipeline = ::vk::pipeline(m_device, pipeline_configuration);

            m_begin_initialize = false;
        }

        // TODO: Fix this when getting shader hot-reloading to workagain.
        // if (m_shader_group.reload_requested()) {
        // 	console_log_info("reloading shaders and graphics pipeline!!");
        // 	m_main_pipeline.destroy();
        // 	std::vector<::vk::shader_handle> modules =
        // m_shader_group.handles();

        // 	::vk::pipeline_settings pipeline_configuration = {
        // 		.renderpass = p_renderpass,
        // 		.shader_modules = modules,
        // 		.vertex_attributes = m_shader_group.vertex_attributes(),
        // 		.vertex_bind_attributes =
        // 		m_shader_group.vertex_bind_attributes(),
        // 		.descriptor_layouts = m_sets_layouts
        // 	};
        // 	m_main_pipeline.create(pipeline_configuration);
        // }

        VkRenderPassBeginInfo renderpass_begin_info = {
            .sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO,
			.pNext = nullptr,
			.renderPass = p_renderpass,
			.renderArea = {
				.offset = {
					.x = 0,
					.y = 0
				},
				.extent = {
					.width = p_settings.width,
					.height = p_settings.height
				},
			},
			.clearValueCount = static_cast<uint32_t>(clear_values.size()),
			.pClearValues = clear_values.data()
        };

        m_current_command_buffer = p_current;
        m_current_command_buffer.begin(
          ::vk::command_usage::simulatneous_use_bit);

        VkViewport viewport = {
            .x = 0.0f,
            .y = 0.0f,
            .width = static_cast<float>(m_window_extent.width),
            .height = static_cast<float>(m_window_extent.height),
            .minDepth = 0.0f,
            .maxDepth = 1.0f,
        };

        vkCmdSetViewport(m_current_command_buffer, 0, 1, &viewport);

        VkRect2D scissor = {
            .offset = { 0, 0 },
            .extent = { m_window_extent.width, m_window_extent.height },
        };

        vkCmdSetScissor(m_current_command_buffer, 0, 1, &scissor);

        // renderpass_begin_info.framebuffer =
        //   m_main_swapchain.active_framebuffer(m_current_frame);
        renderpass_begin_info.framebuffer = p_framebuffer;

        vkCmdBeginRenderPass(m_current_command_buffer,
                             &renderpass_begin_info,
                             VK_SUBPASS_CONTENTS_INLINE);
    }

    // This just returns the arbitrary amount of bytes of the object
    // TODO: Move this into core/utilities.hpp
    template<typename T>
    std::span<uint8_t> to_bytes(T p_data) {
        return std::span<uint8_t>(reinterpret_cast<uint8_t*>(&p_data),
                                  sizeof(p_data));
    }

    void vk_renderer::post_frame() {

        // For now, using this. Will need to remove this before vulkan
        // integration merging into dev This is for testing and to hopefully
        // have a global_ubo for globalized uniforms
        global_ubo global_frame_ubo = { .mvp = m_proj_view };
        std::span<uint8_t> bytes_data = to_bytes(global_frame_ubo);
        m_global_uniforms.update(bytes_data.data());

        ref<world_scope> current_world =
          system_registry::get_world("Editor World");
        ref<scene_scope> current_scene = current_world->get_scene("LevelScene");

        //! TODO: Replace rendertarget3d with a material component
        flecs::query<> query_targets =
          current_scene->query_builder<mesh_source>().build();
        m_main_pipeline.bind(m_current_command_buffer);
        // Bind global camera data here
        m_global_descriptors.bind(
          m_current_command_buffer, m_current_frame, m_main_pipeline.layout());
        query_targets.each([this](flecs::entity p_entity) {
            const transform* transform_component = p_entity.get<transform>();
            const mesh_source* material_component = p_entity.get<mesh_source>();
            m_model = glm::mat4(1.f);
            m_model = glm::translate(m_model, transform_component->position);
            m_model = glm::scale(m_model, transform_component->scale);
            glm::mat4 rotation_mat4 =
              glm::mat4(glm::quat(transform_component->rotation));

            m_model *= rotation_mat4;

            // Mesh used for viking_room - replaced with std::map equivalent
            material_uniform mesh_material_ubo = {
                .model = m_model, .color = material_component->color
            };

            if (m_cached_meshes[p_entity.id()].loaded()) {

                m_cached_meshes[p_entity.id()].update_uniform(
                  mesh_material_ubo);

                m_mesh_descriptors[p_entity.id()]["materials"].bind(
                  m_current_command_buffer,
                  m_current_frame,
                  m_main_pipeline.layout());
                m_cached_meshes[p_entity.id()].draw(m_current_command_buffer);
            }
        });

        vkCmdEndRenderPass(m_current_command_buffer);
        m_current_command_buffer.end();
    }
};
