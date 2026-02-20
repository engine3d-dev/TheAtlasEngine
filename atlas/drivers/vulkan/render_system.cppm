module;

#include <vulkan/vulkan.h>
#include <array>
#include <glm/glm.hpp>
#include <string>
#include <map>
#include <flecs.h>
#include <stdexcept>
#include <filesystem>

#include <glm/ext.hpp>

export module atlas.drivers.vulkan.render_system;
import atlas.drivers.renderer_system;

import atlas.core.utilities;
import vk;
import atlas.core.scene;
import atlas.drivers.graphics_context;
import atlas.drivers.vulkan.instance_context;
import atlas.drivers.vulkan.physical_device;
import atlas.drivers.vulkan.device;
import atlas.drivers.vulkan.mesh;
import atlas.drivers.vulkan.shader_resource_group;
import atlas.core.scene.world;
import atlas.core.scene;
import atlas.drivers.vulkan.uniforms;
import atlas.drivers.vulkan.mesh;
import atlas.core.scene.components;

export namespace atlas::vulkan {
    /**
     * @brief Something to consider for mesh loading.
     *
     * How will meshes get loaded?
     *
     * There are a few ways for considering for the base approach, which are:
     * TODO: These are things to think about how this may happen because I want
     * to make this lightweight in the sense the data isnt continuously being
     * modified. Taking a looking at minimizing the loading state of vulkan
     * implementation-specific meshes
     *      * While also making the way how mesh components are being added the
     * same as before
     * - Something to avoid is the entities containing the geometry data itself
     * but being able to reference to their respective geometry data that are
     * getting submitted to the GPU
     *
     * * Batching ID's into hash table that contains the actual geometry data
     *  * Using ID to search up the mesh loaded and cached into the map, then
     * reuse that geometry whenever the uniforms are changed
     *  * This way we aren't reloading in the same mesh multiple times, treating
     * the hash table as a slot of the mesh contained within that scene
     *  * Potentially std::map<std::string, std::map<std::string,
     * vk::mesh_metadata>> m_geometries
     *      * Idea is the std::string is the geometries within this scene, the
     * data format is: <scene_str, <mesh_str, mesh_metadata>>
     */
    class render_system : public renderer_system {
    public:
        render_system(ref<graphics_context> p_context,
                      const window_params& p_params,
                      uint32_t p_image_size,
                      const std::string&) {
            m_physical = instance_context::physical_driver();
            m_device = instance_context::logical_device();

            m_window_extent = p_params;
            m_image_count = p_image_size;

#ifdef USE_SHADERC
            console_log_info("shaderc enabled!!");
            std::array<vk::shader_source, 2> shader_sources = {
                vk::shader_source{
                  "builtin.shaders/test.vert",
                  vk::shader_stage::vertex,
                },
                vk::shader_source {
                    "builtin.shaders/test.frag",
                    vk::shader_stage::fragment,
                }
            };
#else
            console_log_info("shaderc disabled!!");
            std::array<vk::shader_source, 2> shader_sources = {
                vk::shader_source{
                  "builtin.shaders/test.vert.spv",
                  vk::shader_stage::vertex,
                },
                vk::shader_source{
                  "builtin.shaders/test.frag.spv",
                  vk::shader_stage::fragment,
                }
            };
#endif
            std::array<vk::vertex_attribute_entry, 4> attribute_entries = {
                vk::vertex_attribute_entry{
                  .location = 0,
                  .format = vk::format::rgb32_sfloat,
                  .stride = offsetof(vk::vertex_input, position),
                },
                vk::vertex_attribute_entry{
                  .location = 1,
                  .format = vk::format::rgb32_sfloat,
                  .stride = offsetof(vk::vertex_input, color),
                },
                vk::vertex_attribute_entry{
                  .location = 2,
                  .format = vk::format::rgb32_sfloat,
                  .stride = offsetof(vk::vertex_input, normals),
                },
                vk::vertex_attribute_entry{
                  .location = 3,
                  .format = vk::format::rg32_sfloat,
                  .stride = offsetof(vk::vertex_input, uv),
                }
            };

            std::array<vk::vertex_attribute, 1> attribute = {
                vk::vertex_attribute{
                  // layout (set = 0, binding = 0)
                  .binding = 0,
                  .entries = attribute_entries,
                  .stride = sizeof(vk::vertex_input),
                  .input_rate = vk::input_rate::vertex,
                },
            };

            vk::shader_resource_info shader_info = {
                .sources = shader_sources,
            };

            try {
                m_shader_group = shader_resource_group(m_device, shader_info);
                m_shader_group.vertex_attributes(attribute);
            }
            catch (std::runtime_error& e) {
                console_log_error("Compilation Error!!");
                console_log_error("{}", e.what());
            }
            // Setting global descriptor set 0
            std::vector<vk::descriptor_entry> set0_entries = {
                vk::descriptor_entry{
                    // specifies "layout (set = 0, binding = 0) uniform GlobalUbo"
                    .type = vk::buffer::uniform,
                    .binding_point = {
                        .binding = 0,
                        .stage = vk::shader_stage::vertex,
                    },
                    .descriptor_count = 1,
                },
                vk::descriptor_entry{
                    // specifies "layout (set = 0, binding = 1) uniform light_ubo"
                    .type = vk::buffer::uniform,
                    .binding_point = {
                        .binding = 1,
                        .stage = vk::shader_stage::fragment,
                    },
                    .descriptor_count = 1,
                },
            };

            // uint32_t image_count = image_count;
            vk::descriptor_layout set0_layout = {
                .slot = 0,
                .max_sets = m_image_count,
                .entries = set0_entries,
            };
            m_global_descriptors =
              vk::descriptor_resource(m_device, set0_layout);

            vk::uniform_params global_info = {
                .phsyical_memory_properties = m_physical.memory_properties(),
                .size_bytes = sizeof(global_ubo),
                .debug_name = "\nm_global_uniforms\n",
                .vkSetDebugUtilsObjectNameEXT =
                  instance_context::get_debug_object_name()
            };
            m_global_uniforms = vk::uniform_buffer(m_device, global_info);

            // setting up our light uniforms as the global uniforms rather then
            // per-object basis
            vk::uniform_params light_ubo_params = {
                .phsyical_memory_properties = m_physical.memory_properties(),
                .size_bytes = sizeof(light_scene_ubo),
            };
            m_point_light_uniforms =
              vk::uniform_buffer(m_device, light_ubo_params);

            std::array<vk::write_buffer, 1> binding0_uniforms = {
                vk::write_buffer{
                  .buffer = m_global_uniforms,
                  .offset = 0,
                  .range = m_global_uniforms.size_bytes(),
                },
            };

            std::array<vk::write_buffer, 1> binding1_uniforms = {
                vk::write_buffer{
                  .buffer = m_point_light_uniforms,
                  .offset = 0,
                  .range = m_point_light_uniforms.size_bytes(),
                },
            };

            std::array<vk::write_buffer_descriptor, 2> set0_write_buffers = {
                vk::write_buffer_descriptor{
                  .dst_binding = 0,
                  .uniforms = binding0_uniforms,
                },
                vk::write_buffer_descriptor{
                  .dst_binding = 1,
                  .uniforms = binding1_uniforms,
                }
            };
            m_global_descriptors.update(set0_write_buffers);

            m_sets_layouts = {
                m_global_descriptors.layout(),
            };

            vk::image_extent extent = {
                .width = 1,
                .height = 1,
            };
            m_white_texture =
              vk::texture(m_device, extent, m_physical.memory_properties());

            p_context->submit_resource_free([this]() {
                console_log_info("vulkan::render_system destructin invoked!!");
                m_white_texture.destroy();
                m_shader_group.destroy();
                m_global_descriptors.destroy();
                m_global_uniforms.destroy();
                m_point_light_uniforms.destroy();
                for (auto& [id, mesh] : m_cached_meshes) {
                    console_log_trace(
                      "Entity \"{}\" Destroyed in vk_renderer!!!", id);
                    mesh.destroy();
                }

                for (auto& [id, uniform] : m_mesh_geometry_set) {
                    uniform.destroy();
                }

                for (auto& [id, material_uniform] : m_mesh_material_set) {
                    material_uniform.destroy();
                }

                for (auto& [key, descriptor_map] : m_mesh_descriptors) {
                    for (auto& [descriptor_type, descriptor] : descriptor_map) {
                        descriptor.destroy();
                    }
                }
                m_main_pipeline.destroy();
            });

            console_log_info(
              "Vulkan-specific implementation constructed successfully!!!");
        }

        ~render_system() override = default;

    private:
        void preload_assets(const VkRenderPass& p_renderpass) override {
            m_final_renderpass = p_renderpass;
            // set 1 -- material uniforms
            // ref<world> current_world = system_registry::get_world("Editor
            // World"); ref<scene> current_scene =
            // current_world->get_scene("LevelScene");

            if (m_current_scene == nullptr) {
                console_log_error("m_current_scene == nullptr!");
                return;
            }
            else {
                console_log_error("m_current_scene != nullptr!");
            }

            flecs::query<> caching =
              m_current_scene->query_builder<mesh_source>().build();

            caching.each([this](flecs::entity p_entity) {
                const mesh_source* target = p_entity.get<mesh_source>();
                mesh new_mesh(std::filesystem::path(target->model_path),
                              target->flip);

                // we do a check if the geometry uniform associated with this
                // game object is valid
                if (!m_mesh_geometry_set.contains(p_entity.id())) {
                    vk::uniform_params geo_info = {
                        .phsyical_memory_properties =
                          m_physical.memory_properties(),
                        .size_bytes = sizeof(material_uniform),
                    };
                    m_mesh_geometry_set[p_entity.id()] =
                      vk::uniform_buffer(m_device, geo_info);
                }

                // check if material is already associated with this particular
                // game object
                if (!m_mesh_material_set.contains(p_entity.id())) {
                    vk::uniform_params mat_info = {
                        .phsyical_memory_properties =
                          m_physical.memory_properties(),
                        .size_bytes = sizeof(material_metadata),
                    };
                    m_mesh_material_set[p_entity.id()] =
                      vk::uniform_buffer(m_device, mat_info);
                }

                new_mesh.add_diffuse(std::filesystem::path(target->diffuse));
                new_mesh.add_specular(std::filesystem::path(target->specular));

                if (new_mesh.loaded()) {
                    m_cached_meshes.emplace(p_entity.id(), new_mesh);

                    std::vector<vk::descriptor_entry> set1_entries = {
                        vk::descriptor_entry{
                            // specifies "layout (set = 1, binding = 0) uniform geometry_uniform"
                            .type = vk::buffer::uniform,
                            .binding_point = {
                                .binding = 0,
                                .stage = vk::shader_stage::vertex,
                            },
                            .descriptor_count = 1,
                        },
                        vk::descriptor_entry{
                            // specifies "layout (set = 1, binding = 1) uniform sampler2D diffuse_texture"
                            .type = vk::buffer::combined_image_sampler,
                            .binding_point = {
                                .binding = 1,
                                .stage = vk::shader_stage::fragment,
                            },
                            .descriptor_count = 1,
                        },
                        vk::descriptor_entry{
                            // specifies "layout (set = 1, binding = 2) uniform sampler2D specular_texture"
                            .type = vk::buffer::combined_image_sampler,
                            .binding_point = {
                                .binding = 2,
                                .stage = vk::shader_stage::fragment,
                            },
                            .descriptor_count = 1,
                        },
                        vk::descriptor_entry{
                            // specifies "layout (set = 1, binding = 3) uniform sampler2D material_ubo"
                            .type = vk::buffer::uniform,
                            .binding_point = {
                                .binding = 3,
                                .stage = vk::shader_stage::fragment,
                            },
                            .descriptor_count = 1,
                        },
                    };

                    vk::descriptor_layout set1_layout = {
                        .slot = 1,
                        .max_sets = m_image_count,
                        .entries = set1_entries,
                    };

                    m_mesh_descriptors[p_entity.id()].emplace(
                      "materials",
                      vk::descriptor_resource(m_device, set1_layout));

                    // specify to the vk::write_descriptor_buffer
                    std::array<vk::write_buffer, 1> binding0_buffers = {
                        vk::write_buffer{
                          .buffer = m_mesh_geometry_set[p_entity.id()],
                          .offset = 0,
                          .range =
                            m_mesh_geometry_set[p_entity.id()].size_bytes(),
                        }
                    };

                    std::array<vk::write_buffer, 1> binding3_buffers = {
                        vk::write_buffer{
                          .buffer = m_mesh_material_set[p_entity.id()],
                          .offset = 0,
                          .range =
                            m_mesh_material_set[p_entity.id()].size_bytes(),
                        }
                    };

                    std::vector<vk::write_buffer_descriptor>
                      material_uniforms = {
                          // layout(set=  1, binding = 0) geometry_ubo
                          vk::write_buffer_descriptor{
                            .dst_binding = 0,
                            .uniforms = binding0_buffers,
                          },
                          // layout(set=  1, binding = 3) material_ubo
                          vk::write_buffer_descriptor{
                            .dst_binding = 3,
                            .uniforms = binding3_buffers,
                          },
                      };

                    // layout(set = 1, binding = 1)
                    // If the texture loaded successfully then we use that
                    // texture, otherwise utilize the default white texture
                    vk::sample_image diffuse =
                      m_cached_meshes[p_entity.id()].diffuse_loaded()
                        ? m_cached_meshes[p_entity.id()].diffuse()
                        : m_white_texture.image();

                    // layout(set = 1, binding = 2)
                    vk::sample_image specular =
                      m_cached_meshes[p_entity.id()].specular_loaded()
                        ? m_cached_meshes[p_entity.id()].specular()
                        : m_white_texture.image();

                    // writes to texture at layout(set = 1, binding = 1)
                    std::array<vk::write_image, 1> binding1_images = {
                        vk::write_image{
                          .sampler = diffuse.sampler(),
                          .view = diffuse.image_view(),
                          // .image_layout =
                          // VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
                          .layout = vk::image_layout::shader_read_only_optimal,
                        },
                    };

                    // writes to texture at layout(set = 1, binding = 2)
                    std::array<vk::write_image, 1> binding2_images = {
                        vk::write_image{
                          .sampler = specular.sampler(),
                          .view = specular.image_view(),
                          // .image_layout =
                          // VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
                          .layout = vk::image_layout::shader_read_only_optimal,
                        },
                    };

                    // vulkan image descriptors are for writing textures
                    std::vector<vk::write_image_descriptor>
                      material_textures = {
                          // layout(set = 1, binding = 1) uniform sampler2D
                          vk::write_image_descriptor{
                            .dst_binding = 1,
                            .sample_images = binding1_images,
                          },
                          // layout(set = 1, binding = 2) uniform sampler2D
                          vk::write_image_descriptor{
                            .dst_binding = 2,
                            .sample_images = binding2_images,
                          },
                      };

                    m_mesh_descriptors[p_entity.id()]["materials"].update(
                      material_uniforms, material_textures);

                    m_sets_layouts.push_back(
                      m_mesh_descriptors[p_entity.id()]["materials"].layout());
                }
            });

            std::vector<vk::shader_handle> modules = m_shader_group.handles();

            std::array<vk::color_blend_attachment_state, 1>
              color_blend_attachments = {
                  vk::color_blend_attachment_state{},
              };

            std::array<vk::dynamic_state, 2> dynamic_states = {
                vk::dynamic_state::viewport, vk::dynamic_state::scissor
            };

            vk::pipeline_params pipeline_configuration = {
                .renderpass = m_final_renderpass,
                .shader_modules = modules,
                .vertex_attributes = m_shader_group.vertex_attributes(),
                .vertex_bind_attributes =
                  m_shader_group.vertex_bind_attributes(),
                .descriptor_layouts = m_sets_layouts,
                .color_blend = {
                    .attachments = color_blend_attachments,
                },
                .depth_stencil_enabled = true,
                .dynamic_states = dynamic_states,
            };
            m_main_pipeline = vk::pipeline(m_device, pipeline_configuration);

            console_log_warn("graphics pipeline = {}", m_main_pipeline.alive());
        }

        void start_frame(const vk::command_buffer& p_current,
                         const window_params& p_settings,
                         const VkRenderPass& p_renderpass,
                         const VkFramebuffer& p_framebuffer,
                         const glm::mat4& p_proj_view,
                         uint32_t p_current_frame) override {
            m_proj_view = p_proj_view;
            // m_current_frame = application::current_frame();
            m_current_frame = p_current_frame;
            m_final_renderpass = p_renderpass;

            std::array<VkClearValue, 2> clear_values = {};

            clear_values[0].color = m_color;
            clear_values[1].depthStencil = { 1.f, 0 };
            m_window_extent = p_settings;

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
              vk::command_usage::simulatneous_use_bit);

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

            renderpass_begin_info.framebuffer = p_framebuffer;

            vkCmdBeginRenderPass(m_current_command_buffer,
                                 &renderpass_begin_info,
                                 VK_SUBPASS_CONTENTS_INLINE);
        }

        void background_color(const glm::vec4& p_color) override {
            m_color = {
                { p_color.x, p_color.y, p_color.z, p_color.w },
            };
        }

        void post_frame() override {
            // For now, using this. Will need to remove this before vulkan
            // integration merging into dev This is for testing and to hopefully
            // have a global_ubo for globalized uniforms
            global_ubo global_frame_ubo = { .mvp = m_proj_view };

            // TODO: Make to_bytes be part of utilities. This can be useful in
            // sending the amount of bytes in batches for batch-rendering
            // std::span<uint8_t> bytes_data = to_bytes(global_frame_ubo);
            // m_global_uniforms.update(bytes_data.data());
            m_global_uniforms.update(&global_frame_ubo);

            // ref<world> current_world = system_registry::get_world("Editor
            // World"); ref<scene> current_scene =
            // current_world->get_scene("LevelScene");

            // query all entities that have a point light
            flecs::query<point_light> query_point_lights =
              m_current_scene->query_builder<point_light>().build();

            light_scene_ubo test_light = {};
            uint32_t index = 0;
            query_point_lights.each(
              [&index, &test_light](flecs::entity p_entity,
                                    point_light& p_light) {
                  const transform* t = p_entity.get<transform>();
                  p_light.position = t->position;

                  test_light.light_sources[index] = {
                      .position = glm::vec4(p_light.position, 1.f),
                      .color = p_light.color,
                      .attenuation = p_light.attenuation,
                      .constant = p_light.constant,
                      .linear = p_light.linear,
                      .quadratic = p_light.quadratic,
                      .ambient = p_light.ambient,
                      .diffuse = p_light.diffuse,
                      .specular = p_light.specular,
                  };
                  index += 1;
              });
            test_light.num_lights = index;

            m_point_light_uniforms.update(&test_light);

            // query all objects with a specified 3d mesh source
            flecs::query<> query_targets =
              m_current_scene->query_builder<mesh_source>().build();

            m_main_pipeline.bind(m_current_command_buffer);

            // Bind global camera data here
            m_global_descriptors.bind(m_current_command_buffer,
                                      m_main_pipeline.layout());
            query_targets.each([this](flecs::entity p_entity) {
                const transform* transform_component =
                  p_entity.get<transform>();
                const mesh_source* material_component =
                  p_entity.get<mesh_source>();
                m_model = glm::mat4(1.f);
                m_model =
                  glm::translate(m_model, transform_component->position);
                m_model = glm::scale(m_model, transform_component->scale);
                glm::mat4 rotation_mat4 =
                  glm::mat4(glm::quat(transform_component->rotation));

                m_model *= rotation_mat4;

                // Mesh used for viking_room - replaced with std::map equivalent
                geometry_uniform mesh_ubo = { .model = m_model,
                                              .color =
                                                material_component->color };

                if (m_cached_meshes[p_entity.id()].loaded()) {
                    m_mesh_geometry_set[p_entity.id()].update(&mesh_ubo);

                    material_metadata data = {};

                    if (p_entity.has<material_metadata>()) {
                        data = *p_entity.get<material_metadata>();
                    }
                    m_mesh_material_set[p_entity.id()].update(&data);

                    m_mesh_descriptors[p_entity.id()]["materials"].bind(
                      m_current_command_buffer, m_main_pipeline.layout());

                    m_cached_meshes[p_entity.id()].draw(
                      m_current_command_buffer);
                }
            });

            vkCmdEndRenderPass(m_current_command_buffer);
            // Do not end the command buffer here when using offscreen +
            // swapchain two-pass: the application will begin the swapchain
            // render pass for ImGui, then end the command buffer.
            // m_current_command_buffer.end();
        }

        void current_scene(ref<scene> p_scene_ctx) override {
            m_current_scene = p_scene_ctx;
        }

    private:
        VkDevice m_device = nullptr;
        physical_device m_physical;
        glm::mat4 m_proj_view;
        VkRenderPass m_final_renderpass = nullptr;
        window_params m_window_extent;
        vk::command_buffer m_current_command_buffer{};
        VkClearColorValue m_color;

        uint32_t m_image_count = 0;
        shader_resource_group m_shader_group;
        vk::pipeline m_main_pipeline;
        vk::descriptor_resource m_global_descriptors;
        std::vector<VkDescriptorSetLayout> m_sets_layouts;

        std::map<uint32_t, mesh> m_cached_meshes;
        vk::uniform_buffer m_global_uniforms;
        vk::uniform_buffer m_point_light_uniforms;

        // game object-specific meshes
        std::map<uint32_t, vk::uniform_buffer> m_mesh_geometry_set;
        // TODO: Make this into a material system, eventually
        std::map<uint64_t, vk::uniform_buffer> m_mesh_material_set;
        std::map<uint32_t, std::map<std::string, vk::descriptor_resource>>
          m_mesh_descriptors;
        uint32_t m_current_frame = 0;
        glm::mat4 m_model = { 1.f };

        vk::texture m_white_texture;

        ref<scene> m_current_scene;
    };
};
