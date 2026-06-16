module;

#include <memory>
#include <array>
#include <span>
#include <unordered_map>
#include <optional>
#include <filesystem>

#include <vulkan/vulkan.h>
#include <flecs.h>
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/hash.hpp>

#include <mutex>
#include <future>
#include <deque>

#include <chrono>

#include <taskflow/taskflow.hpp>
#include <taskflow/algorithm/for_each.hpp>

export module atlas.drivers.vulkan:render_context;

import :stb_image;
import :graphics_context;
import :environment_map;
import :gpu_uniforms;

import atlas.core.scene;
import atlas.drivers.importer;
import atlas.core.utilities;

import vk;

export namespace atlas {
    enum class material_type : uint8_t {
        none,
        diffuse,
        specular
    };
    struct request_payload {
        uint64_t entity_id=0;
        // gpu_mesh_data mesh{};
        std::optional<obj_importer> obj;
        std::optional<gltf_importer> gltf;
    };

    struct mesh_task {
        uint64_t entity_id=0;
        std::string path;
        std::optional<obj_importer> obj;
        std::optional<gltf_importer> gltf;
        bool duplicate=false;
        bool flip=false;

    };

    struct request_material_payload {
        uint64_t entity_id=0;
        std::optional<stb_image> diffuse;
        std::optional<stb_image> specular;
    };
    /**
     *
     * @brief Context that translates the ECS rendering-specific components to
     * GPU-visible resources
     *
     * Manages the dispatching of task workloads to performing data transfers
     * and draw calls.
     *
     */
    class render_context {
    public:
        render_context() = default;
        render_context(/*NOLINT*/ std::shared_ptr<graphics_context> p_context,
                       VkFormat p_color_format,
                       VkFormat p_depth_format) {
            m_physical = p_context->physical_device();
            m_device = p_context->logical_device();
            m_color_format = p_color_format;
            m_depth_format = p_depth_format;

            // Vertex Attributes Parameters
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
                  .format = vk::format::rg32_sfloat,
                  .stride = offsetof(vk::vertex_input, uv),
                },
                vk::vertex_attribute_entry{
                  .location = 3,
                  .format = vk::format::rgb32_sfloat,
                  .stride = offsetof(vk::vertex_input, normals),
                }
            };
            std::array<vk::vertex_attribute, 1> attributes = {
                vk::vertex_attribute{
                  .binding = 0,
                  .entries = attribute_entries,
                  .stride = sizeof(vk::vertex_input),
                  .input_rate = vk::input_rate::vertex,
                },
            };
            std::array<vk::shader_source, 2> shader_sources = {
                vk::shader_source{
                  .filename = "builtin.shaders/pbr.vert.spv",
                  .stage = vk::shader_stage::vertex,
                },
                vk::shader_source{
                  .filename = "builtin.shaders/pbr.frag.spv",
                  .stage = vk::shader_stage::fragment,
                },
            };

            // To render triangle, we do not need to set any vertex attributes
            vk::shader_resource_info shader_info = {
                .sources = shader_sources,
            };
            m_shader_resource = vk::shader_resource(*m_device, shader_info);
            m_shader_resource.vertex_attributes(attributes);

            // Configuring Descriptor Set 0 -- specify to vk::pipeline
            // Descriptor Set 0
            uint32_t max_descriptors = 1000;
            std::array<vk::descriptor_entry, 2> entries_set1 = {
                vk::descriptor_entry{
                    // layout (set = 0, binding = 1) uniform sampler2D textures[];
                    .type = vk::descriptor_type::combined_image_sampler,
                    .binding_point = {
                        .binding = 1,
                        .stage = vk::shader_stage::fragment,
                    },
                    .descriptor_count = max_descriptors,
                    .flags = vk::descriptor_bind_flags::partially_bound_bit |
                            vk::descriptor_bind_flags::update_after_bind,
                },
                vk::descriptor_entry{
                    // layout (set = 0, binding = 2) uniform sampler2D environment_map;
                    .type = vk::descriptor_type::combined_image_sampler,
                    .binding_point = {
                        .binding = 2,
                        .stage = vk::shader_stage::fragment,
                    },
                    .descriptor_count = 1,
                    .flags = vk::descriptor_bind_flags::partially_bound_bit |
                            vk::descriptor_bind_flags::update_after_bind,
                }
            };

            // layout(set = 0, ...)
            vk::descriptor_layout set0_layout = {
                .slot = 0,
                .max_sets = max_descriptors,
                .entries =
                  entries_set1, // descriptor layout entries description
                .descriptor_counts =
                  std::span<const uint32_t>(&max_descriptors, 1),
            };
            m_set0_resource = vk::descriptor_resource(
              *m_device,
              set0_layout,
              vk::descriptor_layout_flags::update_after_bind_pool);

            std::array<vk::color_blend_attachment_state, 1>
              color_blend_attachments = {
                  vk::color_blend_attachment_state{},
              };

            std::array<vk::dynamic_state, 2> dynamic_states = {
                vk::dynamic_state::viewport,
                vk::dynamic_state::scissor,
            };

            m_format = static_cast<uint32_t>(p_color_format);
            uint32_t vertex_mask =
              static_cast<uint32_t>(vk::shader_stage::vertex);
            uint32_t fragment_mask =
              static_cast<uint32_t>(vk::shader_stage::fragment);
            uint32_t stage_mask = vertex_mask | fragment_mask;
            m_stage = static_cast<vk::shader_stage>(stage_mask);
            vk::push_constant_range range = {
                .stage = m_stage,
                .offset = 0,
                .range = sizeof(push_constant_data),
            };

            VkDescriptorSetLayout descriptor0_layout = m_set0_resource.layout();
            vk::pipeline_params pipeline_configuration = {
                .use_render_pipeline = true,
                .color_attachment_formats = std::span<const uint32_t>(&m_format, 1),
                .depth_format = static_cast<uint32_t>(p_depth_format),
                .stencil_format = static_cast<uint32_t>(p_depth_format),
                .renderpass = nullptr,
                .shader_modules = m_shader_resource.handles(),
                .vertex_attributes = m_shader_resource.vertex_attributes(),
                .vertex_bind_attributes = m_shader_resource.vertex_bind_attributes(),
                .descriptor_layouts = std::span<VkDescriptorSetLayout>(&descriptor0_layout, 1),
                .color_blend = {
                    .attachments = color_blend_attachments,
                },
                .depth_stencil_enabled = true,
                .dynamic_states = dynamic_states,
                .push_constants = std::span<const vk::push_constant_range>(&range, 1),
            };
            m_main_pipeline = vk::pipeline(*m_device, pipeline_configuration);

            vk::buffer_parameters uniform_params = {
                .memory_mask = m_physical->memory_properties(
                  vk::memory_property::host_visible_bit |
                  vk::memory_property::host_cached_bit),
                .usage = vk::buffer_usage::uniform_buffer_bit |
                         vk::buffer_usage::shader_device_address_bit,
                .allocate_flags =
                  vk::memory_allocate_flags::device_address_bit_khr,
            };

            m_scene_uniforms = vk::dyn::buffer(
              *m_device, sizeof(scene_uniforms), uniform_params);

            // We are setting to the maximum of objects that should be managed
            // in this uniform buffer This uniform buffer is responsible for
            // managing the model matrices of every object in a given scene.
            uint32_t max_objects = 10'000;
            m_object_model_uniforms = vk::dyn::buffer(
              *m_device, sizeof(objects_uniform) * max_objects, uniform_params);

            vk::buffer_parameters indirect_buffer_params = {
                .memory_mask = m_physical->memory_properties(
                  vk::memory_property::host_visible_bit |
                  vk::memory_property::host_cached_bit),
                .usage = vk::buffer_usage::storage_buffer_bit |
                         vk::buffer_usage::indirect_buffer_bit |
                         vk::buffer_usage::shader_device_address_bit,
                .allocate_flags =
                  vk::memory_allocate_flags::device_address_bit_khr,
            };
            m_indirect_buffer =
              vk::dyn::buffer(*m_device,
                              sizeof(VkDrawIndirectCommand) * max_objects,
                              indirect_buffer_params);

            // configuring uniforms for point lights
            m_lighting_uniforms = vk::dyn::buffer(
              *m_device, sizeof(light_scene_ubo), uniform_params);

            // Index 0 will default to a white texture
            vk::image_extent extent = {
                .width = 1,
                .height = 1,
            };
            std::array<uint8_t, 4> white_color = { 0xff, 0xff, 0xff, 0xff };
            m_gpu_textures.emplace_back(
              *m_device,
              extent,
              white_color,
              m_physical->memory_properties(
                vk::memory_property::host_visible_bit |
                vk::memory_property::host_cached_bit));


            m_executor = std::make_shared<tf::Executor>();
        }

        void prebake() {
            flecs::query<> all_meshes =
              m_world->query_builder<mesh_source>().build();

            auto start_time = std::chrono::high_resolution_clock::now();
            tf::Taskflow taskflow;
            std::vector<mesh_task> group_tasks;
            group_tasks.reserve(all_meshes.count());

            all_meshes.each([&](flecs::entity p_entity) {
                const mesh_source* src = p_entity.get<mesh_source>();
                mesh_task spawn_task={};
                spawn_task.entity_id = p_entity.id();
                spawn_task.path = src->model_path;
                spawn_task.flip = src->flip;
                std::filesystem::path path = std::filesystem::path(src->model_path);
                const std::string ext = path.extension().string();
                group_tasks.emplace_back(spawn_task);

                mesh_task* task = &group_tasks.back();

                gpu_draw_call gpu_mesh{};


                vk::buffer_parameters vertex_params = {
                    .memory_mask = m_physical->memory_properties(vk::memory_property::host_visible_bit | vk::memory_property::host_cached_bit),
                    .usage = vk::buffer_usage::transfer_dst_bit | vk::buffer_usage::vertex_buffer_bit,
                };
                vk::buffer_parameters index_params = {
                    .memory_mask = m_physical->memory_properties(vk::memory_property::host_visible_bit | vk::memory_property::host_cached_bit),
                    .usage = vk::buffer_usage::index_buffer_bit,
                };

                tf::Task processing_task = taskflow.emplace([task, ext]() mutable{
                    if(ext == ".obj") {
                        task->obj.emplace(task->path, task->flip);
                    }

                    if(ext == ".gltf" or ext == ".glb") {
                        task->gltf.emplace(task->path, task->flip);
                    }
                });


                tf::Task loading_task = taskflow.emplace([gpu_mesh, task, this, vertex_params, index_params]() mutable {
                    bool successful = false;
                    if (task->obj.has_value()) {
                        auto& importer = task->obj.value();
                        if (!importer.vertices().empty()) {
                            gpu_mesh.has_indices_buffer = !importer.indices().empty();
                            gpu_mesh.vertices_size = importer.vertices().size();
                            gpu_mesh.indices_size = importer.indices().size();

                            m_cached_vertexes.emplace(task->path, vk::vertex_buffer(*m_device, importer.vertices(), vertex_params));
                            m_cacched_index_buffers.emplace(task->path, vk::index_buffer(*m_device, importer.indices(), index_params));
                            successful = true;
                        }
                    }
                    if (task->gltf.has_value()) {
                        auto& importer = task->gltf.value();
                        if (!importer.vertices().empty()) {
                            gpu_mesh.has_indices_buffer = !importer.indices().empty();
                            gpu_mesh.vertices_size = importer.vertices().size();
                            gpu_mesh.indices_size = importer.indices().size();

                            m_cached_vertexes.emplace(task->path, vk::vertex_buffer(*m_device, importer.vertices(), vertex_params));
                            m_cacched_index_buffers.emplace(task->path, vk::index_buffer(*m_device, importer.indices(), index_params));
                            successful = true;
                        }
                    }

                    if(successful) {
                        // std::lock_guard<std::mutex> guard(mutex);
                        m_meshes.emplace(task->entity_id, gpu_mesh);
                    }
                });


                processing_task.precede(loading_task);
            });

            tf::Future<void> future = m_executor->run(taskflow);
            future.wait();

            auto current_time = std::chrono::high_resolution_clock::now();

            uint64_t elapsed_ms = std::chrono::duration_cast<std::chrono::milliseconds>(current_time - start_time).count();
            uint64_t elapsed_sec = std::chrono::duration_cast<std::chrono::seconds>(current_time - start_time).count();
            console_log_info("Elapsed Seconds: {}", elapsed_sec);
            console_log_info("Elapsed Microseconds: {}", elapsed_ms);


            all_meshes.each([this](flecs::entity p_entity) {
                const mesh_source* src = p_entity.get<mesh_source>();
                // vk::texture_params config_texture = {
                //     .memory_mask = m_physical->memory_properties(
                //       vk::memory_property::host_visible_bit |
                //       vk::memory_property::host_cached_bit),
                // };
                // // // Loading texture and setting up VkSampler and VkImageView
                // stb_image diffuse_img = stb_image(src->diffuse, config_texture);
                // stb_image specular_img = stb_image(src->specular, config_texture);

                // // Reminder: Use diffuse_idx
                gpu_material material = {};
                // if (!src->diffuse.empty()) {
                //     material.diffuse_idx = m_texture_slot_index++;
                //     m_gpu_textures.emplace_back(
                //       *m_device, &diffuse_img, config_texture);
                // }

                // if (!src->specular.empty()) {
                //     material.specular_idx = m_texture_slot_index++;
                //     m_gpu_textures.emplace_back(
                //       *m_device, &specular_img, config_texture);
                // }

                m_material_table.emplace(p_entity.id(), material);
            });

            // Preparing the texture data before we update descriptor set 0
            // Storing all of our texture via one contiguous array of textures
            for (auto& image : m_gpu_textures) {
                vk::write_image viking_room_texture = {
                    .sampler = image.image().sampler(),
                    .view = image.image().image_view(),
                    .layout = vk::image_layout::shader_read_only_optimal,
                };
                m_gpu_images.emplace_back(viking_room_texture);
            }

            // Ensure that we load the environment
            // We should never load an invalid environment
            if (m_world->has<environment>()) {
                const environment* environment_data =
                  m_world->get<environment>();
                m_environment_map = environment_map(m_device,
                                                    m_physical.value(),
                                                    environment_data->filepath,
                                                    m_color_format,
                                                    m_depth_format);
            }
            else {
                std::array<float, 4> black_color = { 0.f, 0.f, 0.f, 0.f };
                m_environment_map =
                  environment_map(m_device,
                                  m_physical.value(),
                                  black_color,
                                  vk::image_extent{ .width = 1, .height = 1 },
                                  m_color_format,
                                  m_depth_format);
            }

            vk::write_image environment_image = {
                .sampler = m_environment_map.sampler(),
                .view = m_environment_map.image_view(),
                .layout = vk::image_layout::shader_read_only_optimal,
            };
            std::array<vk::write_image_descriptor, 2> set0_samples = {
                // layout(set = 0, binding = 1) sampler2D[]
                vk::write_image_descriptor{
                  .dst_binding = 1,
                  .sample_images = m_gpu_images,
                },
                // layout(set = 0, binding = 2) sampler2D environment;
                vk::write_image_descriptor{
                  .dst_binding = 2,
                  .sample_images =
                    std::span<const vk::write_image>(&environment_image, 1),
                }
            };

            m_set0_resource.update({}, set0_samples);
        }

        void begin(const glm::mat4& p_proj, const glm::mat4& p_view) {
            // Calculating skybox projection and view
            // m_skybox_proj_view = p_proj * glm::mat4(glm::mat3(p_view));
            m_projection = p_proj;
            m_view = p_view;

            // querying for objects with point lights
            flecs::query<point_light> all_point_lights =
              m_world->query_builder<point_light>().build();

            int index = 0;
            light_scene_ubo scene_point_lights{};
            all_point_lights.each(
              [&index, &scene_point_lights](flecs::entity p_entity,
                                            point_light& p_light) {
                  const transform* t = p_entity.get<transform>();
                  p_light.position = t->position;

                  scene_point_lights.point_lights[index] = {
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

                  index++;
              });

            scene_point_lights.num_lights = index;

            m_lighting_uniforms.transfer<light_scene_ubo>(
              std::span<const light_scene_ubo>(&scene_point_lights, 1));

            m_main_pipeline.bind(*m_current_command);

            flecs::query<> all_meshes =
              m_world->query_builder<mesh_source>().build();

            // Camera projection/view matrices calculated for worldspace
            // calculation
            scene_uniforms scene_ubo = {
                .view = p_view,
                .proj = p_proj,
                .camera_pos = m_camera_pos,
            };

            m_scene_uniforms.transfer<scene_uniforms>(
              std::span<scene_uniforms>(&scene_ubo, 1));

            // Calculating model matrix based on object's transforms
            // specifications (pos, scale, rotation)
            all_meshes.each([this](flecs::entity p_entity) {
                const transform* t = p_entity.get<transform>();
                glm::mat4 model = glm::mat4(1.f);
                model = glm::translate(model, t->position);

                glm::mat4 rotation_mat = glm::mat4(glm::quat(t->rotation));
                model *= rotation_mat;
                model = glm::scale(model, t->scale);

                if (m_model_matrices_lookup.contains(p_entity.id())) {
                    // hash table to lookup specific index, using the entitys
                    // main ID has a hash key This way we can use the hash value
                    // as the location in the index to modify that model matrix.
                    m_model_matrices[m_model_matrices_lookup[p_entity.id()]] =
                      model;
                }
                else {
                    // Add model matrix if non existant in the array
                    m_model_matrices.push_back(model);

                    // Keeping track of the location to that model matrix for
                    // book keeping.
                    m_model_matrices_lookup.emplace(
                      p_entity.id(), m_model_matrix_index_count++);
                }
            });

            m_object_model_uniforms.transfer<glm::mat4>(std::span<glm::mat4>(
              m_model_matrices.data(), m_model_matrices.size()));

            const VkDescriptorSet set0 = m_set0_resource;
            m_current_command->bind_descriptors(
              m_main_pipeline.layout(),
              VK_PIPELINE_BIND_POINT_GRAPHICS,
              std::span<const VkDescriptorSet>(&set0, 1));
        }

        void end() {
            flecs::query<> all_meshes =
              m_world->query_builder<mesh_source>().build();
            all_meshes.each([this](flecs::entity p_entity) {
                const mesh_source* src = p_entity.get<mesh_source>();
                // Retrieving the buffer address that can be looked up from the
                // glsl shader
                const uint64_t scene_ubo_address =
                  m_scene_uniforms.get_device_address();
                const uint64_t objects_ubo_address =
                  m_object_model_uniforms.get_device_address();
                const uint64_t lighting_address =
                  m_lighting_uniforms.get_device_address();
                push_constant_data push = {
                    .scene_address = scene_ubo_address,
                    .model_mat_array_address = objects_ubo_address,
                    .model_idx = static_cast<uint32_t>(
                      m_model_matrices_lookup[p_entity.id()]),
                    .diffuse_idx = static_cast<uint32_t>(
                      m_material_table[p_entity.id()].diffuse_idx),
                    .specular_idx = static_cast<uint32_t>(
                      m_material_table[p_entity.id()].specular_idx),
                    .point_light_address = lighting_address,
                };

                m_main_pipeline.push_constant<push_constant_data>(
                  *m_current_command, push, m_stage, 0);

                // TODO: Use Vulkan Indirect Command Draw call for this to
                // reduce draw calls
                const auto& mesh = m_meshes[p_entity.id()];

                const VkBuffer vertex = m_cached_vertexes[src->model_path];
                uint64_t offset = 0;

                if(vertex != nullptr) {
                    m_current_command->bind_vertex_buffers(
                    std::span<const VkBuffer>(&vertex, 1),
                    std::span<const uint64_t>(&offset, 1));
                    if (mesh.has_indices_buffer and m_cacched_index_buffers[src->model_path] != nullptr) {
                        m_current_command->bind_index_buffers32(m_cacched_index_buffers[src->model_path]);
                        vkCmdDrawIndexed(
                        *m_current_command, mesh.indices_size, 1, 0, 0, 0);
                    }
                    else {
                        vkCmdDraw(*m_current_command, mesh.vertices_size, 1, 0, 0);
                    }
                }
            });

            // Draw Environments
            m_environment_map.begin(m_projection, m_view);

            m_environment_map.end();
        }

        void set_command(vk::command_buffer& p_command) {
            m_current_command = &p_command;
            m_environment_map.set_current_command(*m_current_command);
        }

        void current_scene(flecs::world& p_world) { m_world = &p_world; }

        void destruct() {
            m_scene_uniforms.reset();
            m_object_model_uniforms.reset();
            m_lighting_uniforms.reset();
            m_indirect_buffer.reset();

            m_environment_map.destruct();

            // destroying vector<vk::texture>
            for (auto& image : m_gpu_textures) {
                image.destruct();
            }

            for(auto&[id, vertex] : m_cached_vertexes) {
                vertex.destruct();
            }

            for(auto&[id, index] : m_cacched_index_buffers) {
                index.destruct();
            }

            m_set0_resource.destruct();
            m_shader_resource.destruct();
            m_main_pipeline.destruct();
        }

        void set_camera_pos(const glm::vec4& p_camera_pos) {
            m_camera_pos = p_camera_pos;
        }


    private:
        void async_request_load(uint64_t p_entity_id, const std::string& p_path, bool p_flip) {
            std::future<request_payload> task = std::async(std::launch::async, [p_entity_id, p_path, p_flip](){
                request_payload payload{};
                payload.entity_id = p_entity_id;
                
                if (std::filesystem::path(p_path).extension() == ".obj") {
                    obj_importer importer(p_path, p_flip);
                    payload.obj = importer;
                }
                else if (std::filesystem::path(p_path).extension() ==
                            ".gltf" ||
                            std::filesystem::path(p_path).extension() ==
                            ".glb") {
                    gltf_importer importer(p_path, p_flip);
                    payload.gltf = importer;
                }
                return payload;
            });

            m_async_queue.emplace_back(std::move(task));
        }

        void async_request_material(uint64_t p_id, const mesh_source* p_src) {
            std::future<request_material_payload> task = std::async(std::launch::async, [this, p_id, p_src](){
                std::string diffuse = p_src->diffuse;
                std::string specular = p_src->specular;
                request_material_payload payload{};
                vk::texture_params config_texture = {
                    .memory_mask = m_physical->memory_properties(
                      vk::memory_property::host_visible_bit |
                      vk::memory_property::host_cached_bit),
                };

                payload.entity_id = p_id;
                // stb_image diffuse = stb_image(diffuse, config_texture);
                payload.diffuse = stb_image(diffuse, config_texture);

                payload.specular = stb_image(specular, config_texture);

                return payload;
            });

            m_async_materials_queue.emplace_back(std::move(task));
        }

    private:
        glm::vec4 m_camera_pos = glm::vec4(1.f);
        glm::mat4 m_projection;
        glm::mat4 m_view;
        uint32_t m_format;
        VkFormat m_color_format;
        VkFormat m_depth_format;
        std::optional<vk::physical_device> m_physical;
        std::shared_ptr<vk::device> m_device;
        vk::command_buffer* m_current_command = nullptr;
        vk::shader_resource m_shader_resource;
        vk::pipeline m_main_pipeline;

        std::vector<VkDrawIndirectCommand> m_indirect_commands;
        vk::dyn::buffer m_indirect_buffer;

        vk::descriptor_resource m_set0_resource;

        /**
         * 3 Specific Buffers for accessing data
         * 1.) Scene Uniform Buffer (proj/view)
         * 2.) Object Uniforms (model matrix): Another use is for instancing
         * having multiple mat4's referencing to instancing copies 3.) Material
         * Uniforms (diffuse/specular/etc...)
         */
        vk::dyn::buffer m_scene_uniforms;
        // uniform buffer to write all of our objects mat4 model matrices in
        vk::dyn::buffer m_object_model_uniforms;
        vk::dyn::buffer m_lighting_uniforms;

        std::unordered_map<uint64_t, gpu_draw_call> m_meshes;

        uint64_t m_texture_slot_index = 1;

        // Represents the index to retrieve the location to access the model
        // matrix
        //  inside of the vector<glm::mat4> array
        uint64_t m_model_matrix_index_count = 0;

        // <entity_id, model_matrix_arr_index>
        std::unordered_map<uint64_t, uint64_t> m_model_matrices_lookup;

        std::shared_ptr<tf::Executor> m_executor;

        // Caching already loaded
        // <filepath, mesh_task>
        // This can be used to cache in meshes and since the lookup uses entity ID
        // We can reuse the mesh from that specific entity mesh ID that uses that filepath
        std::unordered_map<std::string, mesh_task> m_cached_meshes_task;
        std::unordered_map<std::string, std::string> m_texture_filepath;
        std::unordered_map<std::string, vk::vertex_buffer> m_cached_vertexes;
        std::unordered_map<std::string, vk::index_buffer> m_cacched_index_buffers;
        std::vector<glm::mat4> m_model_matrices;

        // material lookups
        // <entity_id, gpu_material> is to search for specific indices that
        // correspond to various material surfaces indices to search inside of
        // vector<vk::texture>
        std::unordered_map<uint64_t, gpu_material> m_material_table;
        std::vector<vk::texture> m_gpu_textures;
        std::vector<vk::write_image> m_gpu_images;
        vk::shader_stage m_stage;

        flecs::world* m_world = nullptr;

        environment_map m_environment_map;

        // std::mutex m_mutex;
        std::deque<std::future<request_payload>> m_async_queue;
        std::deque<std::future<request_material_payload>> m_async_materials_queue;
    };
};