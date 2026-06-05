module;

#include <memory>
#include <print>
#include <array>
#include <span>
#include <unordered_map>
#include <optional>

#include <vulkan/vulkan.h>
#include <flecs.h>
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/hash.hpp>
export module atlas.drivers.vulkan:render_context;
import atlas.core.scene;

import atlas.drivers.importer;
import atlas.core.scene;
import atlas.core.scene.components;
import atlas.drivers.vulkan.stb_image;
import atlas.core.utilities;

import :graphics_context;
import vk;

export namespace atlas {

    struct gpu_mesh_data {
        vk::vertex_buffer vertex;
        vk::index_buffer index;
        uint32_t index_count=0;
        uint32_t instance=1;
        uint32_t first_index=0;
        uint32_t vertex_offset=0;
        uint32_t first_instance=0;
        bool has_indices_buffer=false;
        uint32_t vertices_size = 0;
        uint32_t indices_size=0;
    };

    struct push_constant_data {
        uint64_t scene_address=0;
        uint64_t model_mat_array_address=0;
        uint32_t model_idx=0;
        uint32_t material_address=0;
    };

    struct scene_uniforms {
        glm::mat4 view=glm::mat4(1.f);
        glm::mat4 proj=glm::mat4(1.f);
    };

    struct objects_uniform {
        std::span<glm::mat4> model_matrices;
    };

    struct gpu_material {
        uint64_t diffuse_idx=0;
    };


    /**
     * 
     * @brief slot is the index into the texture array to retrieve this specific texture data
     * 
     * vk::texture is the texture data to configure the GPU-visible image resource
    */
    struct gpu_image {
        uint64_t slot=0;
        vk::texture texture_data;
    };

    /**
     * @brief Core render context to schedule images and barriers for coordinating rendering operations
     * 
     * Manages multiple render contexts
    */
    class render_context {
    public:
        render_context() = default;
        render_context(/*NOLINT*/std::shared_ptr<graphics_context> p_context, VkFormat p_color_format, VkFormat p_depth_format) {
            m_physical = p_context->physical_device();
            m_device = p_context->logical_device();

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
            std::array<vk::descriptor_entry, 1> entries_set1 = {
                vk::descriptor_entry{
                    // layout (set = 0, binding = 1) uniform sampler2D textures[];
                    .type = vk::descriptor_type::combined_image_sampler,
                    .binding_point = {
                        .binding = 1,
                        .stage = vk::shader_stage::fragment,
                    },
                    .descriptor_count = max_descriptors,
                    .flags = vk::descriptor_bind_flags::partially_bound_bit |
                            vk::descriptor_bind_flags::variable_descriptor_count_bit |
                            vk::descriptor_bind_flags::update_after_bind,
                }
            };

            // layout(set = 0, ...)
            vk::descriptor_layout set0_layout = {
                .slot = 0,
                .max_sets = max_descriptors,
                .entries = entries_set1, // descriptor layout entries description
                .descriptor_counts = std::span<const uint32_t>(&max_descriptors, 1),
            };
            m_set0_resource = vk::descriptor_resource(*m_device, set0_layout, vk::descriptor_layout_flags::update_after_bind_pool);

            std::array<vk::color_blend_attachment_state, 1> color_blend_attachments = {
                vk::color_blend_attachment_state{},
            };

            std::array<vk::dynamic_state, 2> dynamic_states = {
                vk::dynamic_state::viewport, vk::dynamic_state::scissor,
            };

            m_format = static_cast<uint32_t>(p_color_format);
            uint32_t vertex_mask = static_cast<uint32_t>(vk::shader_stage::vertex);
            uint32_t fragment_mask = static_cast<uint32_t>(vk::shader_stage::fragment);
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
                .memory_mask = m_physical->memory_properties(vk::memory_property::host_visible_bit | vk::memory_property::host_cached_bit),
                .usage = vk::buffer_usage::uniform_buffer_bit | vk::buffer_usage::shader_device_address_bit,
                .allocate_flags = vk::memory_allocate_flags::device_address_bit_khr,
            };

            m_scene_uniforms = vk::dyn::buffer(*m_device, sizeof(scene_uniforms), uniform_params);

            // We are setting to the maximum of objects that should be managed in this uniform buffer
            // This uniform buffer is responsible for managing the model matrices of every object in a given scene.
            uint32_t max_objects = 10'000;
            m_object_model_uniforms = vk::dyn::buffer(*m_device, sizeof(objects_uniform) * max_objects, uniform_params);

            // Index 0 will default to a white texture
            vk::image_extent extent = {
                .width = 1,
                .height = 1,
            };
            std::array<uint8_t, 4> white_color = {0xff, 0xff, 0xff, 0xff};
            m_gpu_textures.emplace_back(*m_device,  extent, white_color, m_physical->memory_properties(vk::memory_property::host_visible_bit | vk::memory_property::host_cached_bit));

        }

        void prebake() {
            flecs::query<> all_meshes = m_world->query_builder<mesh_source>().build();

            all_meshes.each([this](flecs::entity p_entity){
                const mesh_source* src = p_entity.get<mesh_source>();

                vk::buffer_parameters vertex_params = {
                    .memory_mask = m_physical->memory_properties(vk::memory_property::host_visible_bit | vk::memory_property::host_cached_bit),
                    .usage = vk::buffer_usage::transfer_dst_bit | vk::buffer_usage::vertex_buffer_bit,
                };

                vk::buffer_parameters index_params = {
                    .memory_mask = m_physical->memory_properties(vk::memory_property::host_visible_bit | vk::memory_property::host_cached_bit),
                    .usage = vk::buffer_usage::index_buffer_bit,
                };

                // importing .obj 3d models here
                obj_importer importer(src->model_path);
                gpu_mesh_data gpu_mesh{};
                gpu_mesh.vertex = vk::vertex_buffer(*m_device, importer.vertices(), vertex_params);
                gpu_mesh.index = vk::index_buffer(*m_device, importer.indices(), index_params);
                gpu_mesh.has_indices_buffer = (importer.indices().size() >= 0) ? true : false;
                gpu_mesh.vertices_size = importer.vertices().size();
                gpu_mesh.indices_size = importer.indices().size();

                m_meshes.emplace(p_entity.id(), gpu_mesh);
                
                vk::texture_params config_texture = {
                    .memory_mask = m_physical->memory_properties(vk::memory_property::host_visible_bit | vk::memory_property::host_cached_bit),
                };
                // Loading texture and setting up VkSampler and VkImageView
                stb_image diffuse_img = stb_image(src->diffuse, config_texture);
                stb_image specular_img = stb_image(src->specular, config_texture);

                // Reminder: Use diffuse_idx
                gpu_material material = {};
                if(!src->diffuse.empty()) {
                    material.diffuse_idx = m_texture_slot_index++;
                    m_gpu_textures.emplace_back(*m_device, &diffuse_img, config_texture);
                }

                m_material_table.emplace(p_entity.id(), material);
                /*
                if(!src->specular.empty()) {
                    std::println("Loading specular: {}", src->specular);
                    gpu_image specular_store_image {
                        .slot = m_texture_slot_index++,
                        .texture_data = vk::texture(*m_device, &specular_img, config_texture),
                    };

                    m_gpu_storage_images.emplace(p_entity.id(), specular_store_image);
                }
                */
            });

            // Preparing the texture data before we update descriptor set 0
            // Storing all of our texture via one contiguous array of textures
            for(auto& image : m_gpu_textures) {
                vk::write_image viking_room_texture = {
                    .sampler = image.image().sampler(),
                    .view = image.image().image_view(),
                    .layout = vk::image_layout::shader_read_only_optimal,
                };
                m_gpu_images.emplace_back(viking_room_texture);
            }


            std::array<vk::write_image_descriptor, 1> set0_samples = {
                vk::write_image_descriptor{
                    .dst_binding = 1,
                    .sample_images = m_gpu_images,
                }
            };

            m_set0_resource.update({}, set0_samples);

        }

        void begin(const glm::mat4& p_proj, const glm::mat4& p_view) {
            // vk::viewport_params viewport = {
            //     .x = 0.0f,
            //     .y = 0.0f,
            //     .width = static_cast<float>(p_extent.width),
            //     .height = static_cast<float>(p_extent.height),
            //     .min_depth = 0.0f,
            //     .max_depth = 1.0f,
            // };
            // m_current_command->set_viewport(0, 1, std::span<const vk::viewport_params>(&viewport, 1));

            // vk::scissor_params scissor = {
            //     .offset = { 0, 0 },
            //     .extent = {static_cast<uint32_t>(p_extent.width), static_cast<uint32_t>(p_extent.height)},
            // };
            // m_current_command->set_scissor(0, 1, std::span<const vk::scissor_params>(&scissor, 1));

            // m_current_command->begin_rendering(p_begin_params);

            m_main_pipeline.bind(*m_current_command);

            flecs::query<> all_meshes = m_world->query_builder<mesh_source>().build();

            // Camera projection/view matrices calculated for worldspace calculation
            scene_uniforms scene_ubo = {
                .view = p_view,
                .proj = p_proj,
            };
            scene_ubo.proj[1][1] *= -1;

            m_scene_uniforms.transfer<scene_uniforms>(std::span<scene_uniforms>(&scene_ubo, 1));

            // Calculating model matrix based on object's transforms specifications (pos, scale, rotation)
            all_meshes.each([this](flecs::entity p_entity){
                const transform* t = p_entity.get<transform>();
                glm::mat4 model = glm::mat4(1.f);
                model = glm::translate(model, t->position);
                model = glm::scale(model, t->scale);

                if(m_model_matrices_lookup.contains(p_entity.id())) {
                    // hash table to lookup specific index, using the entitys main ID has a hash key
                    // This way we can use the hash value as the location in the index to modify that model matrix.
                    m_model_matrices[m_model_matrices_lookup[p_entity.id()]] = model;
                }
                else {
                    // Add model matrix if non existant in the array
                    m_model_matrices.push_back(model);

                    // Keeping track of the location to that model matrix for book keeping.
                    m_model_matrices_lookup.emplace(p_entity.id(), m_model_matrix_index_count++);
                }
            });

            m_object_model_uniforms.transfer<glm::mat4>(std::span<glm::mat4>(m_model_matrices.data(), m_model_matrices.size()));
            
            const VkDescriptorSet set0 = m_set0_resource;
            m_current_command->bind_descriptors(m_main_pipeline.layout(), VK_PIPELINE_BIND_POINT_GRAPHICS, std::span<const VkDescriptorSet>(&set0, 1));
        }
        
        void end() {
            flecs::query<> all_meshes = m_world->query_builder<mesh_source>().build();
            all_meshes.each([this](flecs::entity p_entity) {
                // Retrieving the buffer address that can be looked up from the glsl shader
                const uint64_t scene_ubo_address = m_scene_uniforms.get_device_address();
                const uint64_t objects_ubo_address = m_object_model_uniforms.get_device_address();
                push_constant_data push = {
                    .scene_address = scene_ubo_address,
                    .model_mat_array_address = objects_ubo_address,
                    .model_idx = static_cast<uint32_t>(m_model_matrices_lookup[p_entity.id()]),
                    .material_address = static_cast<uint32_t>(m_material_table[p_entity.id()].diffuse_idx),
                };

                m_main_pipeline.push_constant<push_constant_data>(*m_current_command, push, m_stage, 0);

                // TODO: Use Vulkan Indirect Command Draw call for this to reduce draw calls
                const auto& mesh = m_meshes[p_entity.id()];
                const VkBuffer vertex = mesh.vertex;
                uint64_t offset = 0;
                m_current_command->bind_vertex_buffers(std::span<const VkBuffer>(&vertex, 1), std::span<const uint64_t>(&offset, 1));
                if (mesh.has_indices_buffer) {
                    m_current_command->bind_index_buffers32(mesh.index);
                    vkCmdDrawIndexed(*m_current_command, mesh.indices_size, 1, 0, 0, 0);
                }
                else {
                    vkCmdDraw(*m_current_command, mesh.vertices_size, 1, 0, 0);
                }
            });
            // m_current_command->end_rendering();
        }

        void set_command(vk::command_buffer& p_command) {
            m_current_command = &p_command;
        }

        void current_scene(flecs::world& p_world) {
            m_world = &p_world;
        }

        void destruct() {
            m_scene_uniforms.reset();
            m_object_model_uniforms.reset();

            // destroying vector<vk::texture>
            for(auto& image : m_gpu_textures) {
                image.destruct();
            }

            for(auto&[id, mesh] : m_meshes) {
                mesh.vertex.destruct();
                mesh.index.destruct();
            }

            m_set0_resource.destruct();
            m_shader_resource.destruct();
            m_main_pipeline.destruct();
        }
    
    private:
        uint32_t m_format;
        std::optional<vk::physical_device> m_physical;
        std::shared_ptr<vk::device> m_device;
        vk::command_buffer* m_current_command=nullptr;
        vk::shader_resource m_shader_resource;
        vk::pipeline m_main_pipeline;
        std::vector<VkDrawIndexedIndirectCommand> m_indirect_commands;
        vk::descriptor_resource m_set0_resource;

        /**
         * 3 Specific Buffers for accessing data
         * 1.) Scene Uniform Buffer (proj/view)
         * 2.) Object Uniforms (model matrix): Another use is for instancing having multiple mat4's referencing to instancing copies
         * 3.) Material Uniforms (diffuse/specular/etc...)
        */
        vk::dyn::buffer m_scene_uniforms;
        // uniform buffer to write all of our objects mat4 model matrices in
        vk::dyn::buffer m_object_model_uniforms;

        std::unordered_map<uint64_t, gpu_mesh_data> m_meshes;

        uint64_t m_texture_slot_index = 1;

        // Represents the index to retrieve the location to access the model matrix
        //  inside of the vector<glm::mat4> array
        uint64_t m_model_matrix_index_count = 0;

        // <entity_id, model_matrix_arr_index>
        std::unordered_map<uint64_t, uint64_t> m_model_matrices_lookup;
        std::vector<glm::mat4> m_model_matrices;


        // material lookups
        // <entity_id, gpu_material> is to search for specific indices that correspond to various material surfaces
        // indices to search inside of vector<vk::texture>
        std::unordered_map<uint64_t, gpu_material> m_material_table;
        std::vector<vk::texture> m_gpu_textures;
        std::vector<vk::write_image> m_gpu_images;
        vk::shader_stage m_stage;

        flecs::world* m_world=nullptr;
    };
};