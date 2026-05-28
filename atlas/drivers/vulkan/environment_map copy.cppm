module;

#include <array>
#include <cstdint>
#include <cstring>
#include <filesystem>
#include <vector>
#include <span>
#include <string>

#define GLFW_INCLUDE_VULKAN
#if _WIN32
#define VK_USE_PLATFORM_WIN32_KHR
#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>
#include <vulkan/vulkan.h>
#else
#include <GLFW/glfw3.h>
#include <vulkan/vulkan.h>
#endif
#include <stb_image.h>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/hash.hpp>
#include <print>

export module atlas.drivers.vulkan.environment_map;

import atlas.drivers.vulkan.instance_context;

import vk;

namespace atlas {

    export struct skybox_uniform {
        glm::mat4 proj_view;
    };

    export class environment_map {
    public:
        environment_map() = default;

        environment_map(const VkDevice& p_device,
                        const std::filesystem::path& p_filename,
                        const vk::physical_device& p_physical,
                        VkRenderPass p_renderpass)
          : m_device(p_device) {
            create_hdr_skybox(p_filename,
                              p_physical,
                              p_physical.memory_properties(
                                vk::memory_property::host_visible_bit |
                                vk::memory_property::host_cached_bit));

            create_skybox_pipeline(p_physical,
                                   p_physical.memory_properties(
                                     vk::memory_property::host_visible_bit |
                                     vk::memory_property::host_cached_bit),
                                   p_renderpass);
        }

        // ~environment_map() {
        //     destroy();
        // }

        void create_hdr_skybox(const std::filesystem::path& p_filename,
                               const vk::physical_device& p_physical,
                               uint32_t p_memory_properties) {

            stbi_set_flip_vertically_on_load(true);
            int w, h, channels;
            float* pixels = stbi_loadf(
              p_filename.string().c_str(), &w, &h, &channels, STBI_rgb_alpha);

            if (!pixels) {
                throw std::runtime_error("Failed to load HDR image at: " +
                                         p_filename.string());
            }

            const uint32_t width = static_cast<uint32_t>(w);
            const uint32_t height = static_cast<uint32_t>(h);

            VkFormat texture_format = VK_FORMAT_R32G32B32A32_SFLOAT;
            const uint64_t bytes_per_pixel_channel = 16; // float are 4 bytes
            const uint64_t total_size_bytes =
              static_cast<uint64_t>(width * height * bytes_per_pixel_channel);
            const uint64_t image_size = total_size_bytes;

            // Creating staging buffer
            // uint32_t property_flag = vk::memory_property::host_visible_bit |
            //                          vk::memory_property::host_cached_bit;
            vk::buffer_parameters staging_buffer_params = {
                // .physical_memory_properties = p_memory_properties,
                .memory_mask = p_memory_properties,
                // .usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
                .usage = vk::buffer_usage::transfer_src_bit,
            };

            vk::buffer staging_buffer =
              vk::buffer(m_device, image_size, staging_buffer_params);

            // Creating image handle to storing the HDR data
            // VkFormat texture_format = VK_FORMAT_R32G32B32A32_SFLOAT;
            vk::image_params skybox_image_params = {
                .extent = {
                    .width = width,
                    .height = height,
                },
                .format = texture_format,
                .memory_mask = p_physical.memory_properties(vk::memory_property::device_local_bit),
                .aspect = vk::image_aspect_flags::color_bit,
                .usage = vk::image_usage::color_attachment_bit,
                .mip_levels = 1,
                .layer_count = 1,
            };
            m_skybox_image = vk::sample_image(m_device, skybox_image_params);

            // Transferring data from the CPU
            // void* data = nullptr;
            // vkMapMemory(m_device, staging_memory, 0, total_size_bytes, 0,
            // &data); std::memcpy(data, pixels,
            // static_cast<size_t>(total_size_bytes)); vkUnmapMemory(m_device,
            // staging_memory);
            std::span<const uint8_t> pixels_data(
              reinterpret_cast<const uint8_t*>(pixels), image_size);
            staging_buffer.transfer(pixels_data, 0);

            // Free CPU pixels immediately after staging copy
            stbi_image_free(pixels);

            // 6. Record and Execute Upload
            vk::command_params upload_params = {
                .levels = vk::command_levels::primary,
                .queue_index = 0, // Graphics Queue
                .flags = vk::command_pool_flags::reset,
            };
            vk::command_buffer upload_cmd(m_device, upload_params);

            upload_cmd.begin(vk::command_usage::one_time_submit);

            // Begin Memory Barrier: Undefined to TRANSFER_DST
            m_skybox_image.memory_barrier(upload_cmd,
                                          texture_format,
                                          VK_IMAGE_LAYOUT_UNDEFINED,
                                          VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);

            // Performing buffer image copies
            // vk::buffer_image_copy image_copy = {
            //     .extent = skybox_image_params.extent
            // };
            vk::buffer_image_copy image_copy{
                .image_offset = { .width = 0, .height = 0, .depth = 0, },
                .image_extent = { .width = skybox_image_params.extent.width, .height = skybox_image_params.extent.height, .depth = 1, },
            };
            staging_buffer.copy_to_image(
              upload_cmd, m_skybox_image, std::span<vk::buffer_image_copy>(&image_copy, 1));

            // Begin Memory Barrier: TRANSFER_DST to SHADER_READ_ONLY
            m_skybox_image.memory_barrier(
              upload_cmd,
              texture_format,
              VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
              VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

            upload_cmd.end();

            VkQueue graphics_queue;
            vkGetDeviceQueue(m_device, 0, 0, &graphics_queue);

            VkCommandBuffer raw_cmd = upload_cmd;
            VkSubmitInfo submit_info = {
                .sType = VK_STRUCTURE_TYPE_SUBMIT_INFO,
                .commandBufferCount = 1,
                .pCommandBuffers = &raw_cmd,
            };

            vkQueueSubmit(graphics_queue, 1, &submit_info, nullptr);
            vkQueueWaitIdle(graphics_queue);

            upload_cmd.destruct();
            staging_buffer.destruct();
            stbi_set_flip_vertically_on_load(false);
        }

        void create_buffers(const vk::physical_device& p_physical) {
             std::vector<vk::vertex_input> vertices = {
                // Front Face
                vk::vertex_input{ { -1.0f, 1.0f, -1.0f },
                                  { 1.0f, 1.0f, 1.0f },
                                  { 0.0f, 0.0f, 0.0f },
                                  { 0.0f, 0.0f } },
                vk::vertex_input{ { -1.0f, -1.0f, -1.0f },
                                  { 1.0f, 1.0f, 1.0f },
                                  { 0.0f, 0.0f, 0.0f },
                                  { 0.0f, 0.0f } },
                vk::vertex_input{ { 1.0f, -1.0f, -1.0f },
                                  { 1.0f, 1.0f, 1.0f },
                                  { 0.0f, 0.0f, 0.0f },
                                  { 0.0f, 0.0f } },
                vk::vertex_input{ { 1.0f, -1.0f, -1.0f },
                                  { 1.0f, 1.0f, 1.0f },
                                  { 0.0f, 0.0f, 0.0f },
                                  { 0.0f, 0.0f } },
                vk::vertex_input{ { 1.0f, 1.0f, -1.0f },
                                  { 1.0f, 1.0f, 1.0f },
                                  { 0.0f, 0.0f, 0.0f },
                                  { 0.0f, 0.0f } },
                vk::vertex_input{ { -1.0f, 1.0f, -1.0f },
                                  { 1.0f, 1.0f, 1.0f },
                                  { 0.0f, 0.0f, 0.0f },
                                  { 0.0f, 0.0f } },

                // Left Face
                vk::vertex_input{ { -1.0f, -1.0f, 1.0f },
                                  { 1.0f, 1.0f, 1.0f },
                                  { 0.0f, 0.0f, 0.0f },
                                  { 0.0f, 0.0f } },
                vk::vertex_input{ { -1.0f, -1.0f, -1.0f },
                                  { 1.0f, 1.0f, 1.0f },
                                  { 0.0f, 0.0f, 0.0f },
                                  { 0.0f, 0.0f } },
                vk::vertex_input{ { -1.0f, 1.0f, -1.0f },
                                  { 1.0f, 1.0f, 1.0f },
                                  { 0.0f, 0.0f, 0.0f },
                                  { 0.0f, 0.0f } },
                vk::vertex_input{ { -1.0f, 1.0f, -1.0f },
                                  { 1.0f, 1.0f, 1.0f },
                                  { 0.0f, 0.0f, 0.0f },
                                  { 0.0f, 0.0f } },
                vk::vertex_input{ { -1.0f, 1.0f, 1.0f },
                                  { 1.0f, 1.0f, 1.0f },
                                  { 0.0f, 0.0f, 0.0f },
                                  { 0.0f, 0.0f } },
                vk::vertex_input{ { -1.0f, -1.0f, 1.0f },
                                  { 1.0f, 1.0f, 1.0f },
                                  { 0.0f, 0.0f, 0.0f },
                                  { 0.0f, 0.0f } },

                // Right Face
                vk::vertex_input{ { 1.0f, -1.0f, -1.0f },
                                  { 1.0f, 1.0f, 1.0f },
                                  { 0.0f, 0.0f, 0.0f },
                                  { 0.0f, 0.0f } },
                vk::vertex_input{ { 1.0f, -1.0f, 1.0f },
                                  { 1.0f, 1.0f, 1.0f },
                                  { 0.0f, 0.0f, 0.0f },
                                  { 0.0f, 0.0f } },
                vk::vertex_input{ { 1.0f, 1.0f, 1.0f },
                                  { 1.0f, 1.0f, 1.0f },
                                  { 0.0f, 0.0f, 0.0f },
                                  { 0.0f, 0.0f } },
                vk::vertex_input{ { 1.0f, 1.0f, 1.0f },
                                  { 1.0f, 1.0f, 1.0f },
                                  { 0.0f, 0.0f, 0.0f },
                                  { 0.0f, 0.0f } },
                vk::vertex_input{ { 1.0f, 1.0f, -1.0f },
                                  { 1.0f, 1.0f, 1.0f },
                                  { 0.0f, 0.0f, 0.0f },
                                  { 0.0f, 0.0f } },
                vk::vertex_input{ { 1.0f, -1.0f, -1.0f },
                                  { 1.0f, 1.0f, 1.0f },
                                  { 0.0f, 0.0f, 0.0f },
                                  { 0.0f, 0.0f } },

                // Back Face
                vk::vertex_input{ { -1.0f, -1.0f, 1.0f },
                                  { 1.0f, 1.0f, 1.0f },
                                  { 0.0f, 0.0f, 0.0f },
                                  { 0.0f, 0.0f } },
                vk::vertex_input{ { -1.0f, 1.0f, 1.0f },
                                  { 1.0f, 1.0f, 1.0f },
                                  { 0.0f, 0.0f, 0.0f },
                                  { 0.0f, 0.0f } },
                vk::vertex_input{ { 1.0f, 1.0f, 1.0f },
                                  { 1.0f, 1.0f, 1.0f },
                                  { 0.0f, 0.0f, 0.0f },
                                  { 0.0f, 0.0f } },
                vk::vertex_input{ { 1.0f, 1.0f, 1.0f },
                                  { 1.0f, 1.0f, 1.0f },
                                  { 0.0f, 0.0f, 0.0f },
                                  { 0.0f, 0.0f } },
                vk::vertex_input{ { 1.0f, -1.0f, 1.0f },
                                  { 1.0f, 1.0f, 1.0f },
                                  { 0.0f, 0.0f, 0.0f },
                                  { 0.0f, 0.0f } },
                vk::vertex_input{ { -1.0f, -1.0f, 1.0f },
                                  { 1.0f, 1.0f, 1.0f },
                                  { 0.0f, 0.0f, 0.0f },
                                  { 0.0f, 0.0f } },

                // Top Face
                vk::vertex_input{ { -1.0f, 1.0f, -1.0f },
                                  { 1.0f, 1.0f, 1.0f },
                                  { 0.0f, 0.0f, 0.0f },
                                  { 0.0f, 0.0f } },
                vk::vertex_input{ { 1.0f, 1.0f, -1.0f },
                                  { 1.0f, 1.0f, 1.0f },
                                  { 0.0f, 0.0f, 0.0f },
                                  { 0.0f, 0.0f } },
                vk::vertex_input{ { 1.0f, 1.0f, 1.0f },
                                  { 1.0f, 1.0f, 1.0f },
                                  { 0.0f, 0.0f, 0.0f },
                                  { 0.0f, 0.0f } },
                vk::vertex_input{ { 1.0f, 1.0f, 1.0f },
                                  { 1.0f, 1.0f, 1.0f },
                                  { 0.0f, 0.0f, 0.0f },
                                  { 0.0f, 0.0f } },
                vk::vertex_input{ { -1.0f, 1.0f, 1.0f },
                                  { 1.0f, 1.0f, 1.0f },
                                  { 0.0f, 0.0f, 0.0f },
                                  { 0.0f, 0.0f } },
                vk::vertex_input{ { -1.0f, 1.0f, -1.0f },
                                  { 1.0f, 1.0f, 1.0f },
                                  { 0.0f, 0.0f, 0.0f },
                                  { 0.0f, 0.0f } },

                // Bottom Face
                vk::vertex_input{ { -1.0f, -1.0f, -1.0f },
                                  { 1.0f, 1.0f, 1.0f },
                                  { 0.0f, 0.0f, 0.0f },
                                  { 0.0f, 0.0f } },
                vk::vertex_input{ { -1.0f, -1.0f, 1.0f },
                                  { 1.0f, 1.0f, 1.0f },
                                  { 0.0f, 0.0f, 0.0f },
                                  { 0.0f, 0.0f } },
                vk::vertex_input{ { 1.0f, -1.0f, -1.0f },
                                  { 1.0f, 1.0f, 1.0f },
                                  { 0.0f, 0.0f, 0.0f },
                                  { 0.0f, 0.0f } },
                vk::vertex_input{ { 1.0f, -1.0f, -1.0f },
                                  { 1.0f, 1.0f, 1.0f },
                                  { 0.0f, 0.0f, 0.0f },
                                  { 0.0f, 0.0f } },
                vk::vertex_input{ { -1.0f, -1.0f, 1.0f },
                                  { 1.0f, 1.0f, 1.0f },
                                  { 0.0f, 0.0f, 0.0f },
                                  { 0.0f, 0.0f } },
                vk::vertex_input{ { 1.0f, -1.0f, 1.0f },
                                  { 1.0f, 1.0f, 1.0f },
                                  { 0.0f, 0.0f, 0.0f },
                                  { 0.0f, 0.0f } }
            };
            const auto property_flags = static_cast<vk::memory_property>(
              vk::memory_property::host_visible_bit |
              vk::memory_property::host_cached_bit);
            vk::buffer_parameters vertex_params = {
                .memory_mask = p_physical.memory_properties(property_flags),
                .property_flags = vk::memory_property::device_local_bit,
                .usage = vk::buffer_usage::transfer_dst_bit |
                         vk::buffer_usage::vertex_buffer_bit,
            };

            m_skybox_vbo = vk::vertex_buffer(m_device, vertices, vertex_params);
        }

        void create_skybox_pipeline(const vk::physical_device& p_physical,
                                    uint32_t p_memory_properties,
                                    const VkRenderPass& p_renderpass) {
            create_buffers(p_physical);
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

            const std::array<vk::shader_source, 2> sources = {
                vk::shader_source{
                  .filename = "builtin.shaders/skybox/skybox.vert.spv",
                  .stage = vk::shader_stage::vertex,
                },
                vk::shader_source{
                  .filename = "builtin.shaders/skybox/skybox.frag.spv",
                  .stage = vk::shader_stage::fragment,
                },
            };

            vk::shader_resource_info shader_info = {
                .sources = sources,
            };
            m_skybox_shaders = vk::shader_resource(m_device, shader_info);
            m_skybox_shaders.vertex_attributes(attribute);

            // set=0 binding=0 UBO: mat4 VP
            // vk::uniform_params ubo_params = {
            //     .memory_mask = p_memory_properties,
            //     .size_bytes = sizeof(skybox_uniform),
            //     .debug_name = "skybox_ubo",
            //     .vkSetDebugUtilsObjectNameEXT = nullptr,
            // };
            // vk::buffer_parameters ubo_params = {
            //     .memory_mask = p_memory_properties,
            // };
            vk::buffer_parameters uniform_params = {
                .memory_mask = p_memory_properties,
                .usage = vk::buffer_usage::uniform_buffer_bit,
            };
            m_skybox_ubo =
              vk::buffer(m_device, sizeof(skybox_uniform), ubo_params);

            skybox_uniform identity = { .proj_view = glm::mat4(1.0f) };
            identity.proj_view[1][1] *= -1;
            // m_skybox_ubo.write(std::span<const skybox_uniform>(&identity,
            // 1));
            m_skybox_ubo.transfer<skybox_uniform>(
              std::span<skybox_uniform>(&identity, 1));

            // set=0 bindings:
            //  - binding 0: UBO (vertex)
            //  - binding 1: samplerCube (fragment)
            std::array<vk::descriptor_entry, 2> entries = {
                vk::descriptor_entry{
                  .type = vk::descriptor_type::uniform,
                  .binding_point =
                    vk::descriptor_binding_point{
                      .binding = 0, .stage = vk::shader_stage::vertex },
                  .descriptor_count = 1,
                },
                vk::descriptor_entry{
                  .type = vk::descriptor_type::combined_image_sampler,
                  .binding_point =
                    vk::descriptor_binding_point{
                      .binding = 1, .stage = vk::shader_stage::fragment },
                  .descriptor_count = 1,
                },
            };

            vk::descriptor_layout desc_layout = {
                .slot = 0,
                .max_sets = 1,
                .entries = entries,
            };
            m_skybox_descriptors =
              vk::descriptor_resource(m_device, desc_layout);

            const std::array<vk::write_buffer, 1> ubo_writes = {
                vk::write_buffer{
                  .buffer = static_cast<VkBuffer>(m_skybox_ubo),
                  .offset = 0,
                  .range = static_cast<uint32_t>(sizeof(skybox_uniform)) },
            };
            const vk::write_buffer_descriptor ubo_write_desc = {
                .dst_binding = 0,
                .uniforms = ubo_writes,
            };

            const std::array<vk::write_image, 1> image_writes = {
                vk::write_image{
                  .sampler = m_skybox_image.sampler(),
                  .view = m_skybox_image.image_view(),
                  .layout = vk::image_layout::shader_read_only_optimal,
                },
            };
            const vk::write_image_descriptor image_write_desc = {
                .dst_binding = 1,
                .sample_images = image_writes,
            };

            m_skybox_descriptors.update(std::span(&ubo_write_desc, 1),
                                        std::span(&image_write_desc, 1));

            const std::array<VkDescriptorSetLayout, 1> layouts = {
                m_skybox_descriptors.layout(),
            };

            const std::array<vk::color_blend_attachment_state, 1>
              blend_attachments = {
                  vk::color_blend_attachment_state{ .blend_enabled = false },
              };
            vk::color_blend_state blend_state = {
                .logic_op_enable = false,
                .logical_op = vk::logical_op::copy,
                .attachments = blend_attachments,
                .blend_constants = {},
            };

            std::array<vk::dynamic_state, 2> dyn = {
                vk::dynamic_state::viewport,
                vk::dynamic_state::scissor,
            };

            // pipeline expects a non-const span<VkDescriptorSetLayout>
            std::array<VkDescriptorSetLayout, 1> pipeline_layouts = layouts;

            vk::pipeline_params pipe_info = {
                .renderpass = p_renderpass,
                .shader_modules = m_skybox_shaders.handles(),
                .vertex_attributes =
                  m_skybox_shaders.vertex_attributes(), // no vertex input
                .vertex_bind_attributes =
                  m_skybox_shaders.vertex_bind_attributes(), // no vertex input
                .descriptor_layouts = pipeline_layouts,
                .input_assembly =
                  vk::input_assembly_state{
                    .topology = vk::primitive_topology::triangle_list,
                    .primitive_restart_enable = false,
                  },
                .viewport =
                  vk::viewport_state{ .viewport_count = 1, .scissor_count = 1 },
                .rasterization =
                  vk::rasterization_state{
                    .polygon_mode = vk::polygon_mode::fill,
                    .cull_mode = vk::cull_mode::front_bit,
                    // .cull_mode = vk::cull_mode::none,
                    // .front_face = vk::front_face::counter_clockwise,
                    .front_face = vk::front_face::clockwise,
                    .line_width = 1.f,
                  },
                .multisample = vk::multisample_state{},
                .color_blend = blend_state,
                .depth_stencil_enabled = true,
                .depth_stencil =
                  vk::depth_stencil_state{
                    .depth_test_enable = true,
                    .depth_write_enable = false,
                    .depth_compare_op = vk::compare_op::less_or_equal,
                    .depth_bounds_test_enable = false,
                    .stencil_test_enable = false,
                  },
                .dynamic_states = dyn,
            };

            m_skybox_pipeline = vk::pipeline(m_device, pipe_info);
        }

        void update_uniform(const skybox_uniform& p_ubo) {
            // m_skybox_ubo.transfer(std::span<const skybox_uniform>(&p_ubo,
            // 1));
            m_skybox_ubo.update(&p_ubo);
        }

        void bind(vk::command_buffer& p_current) {
            m_skybox_pipeline.bind(p_current);
            m_skybox_descriptors.bind(p_current, m_skybox_pipeline.layout());
            // m_skybox_vbo.bind(p_current);
            const VkBuffer vertex = m_skybox_vbo;
            p_current.bind_vertex_buffers(std::span<const VkBuffer>(&vertex, 1));
        }

        void draw(const VkCommandBuffer& p_current) {
            // bind(p_current);
            vkCmdDraw(p_current, m_skybox_vbo.size(), 1, 0, 0);
            // vkCmdDrawIndexed(p_current, 36, 1, 0, 0, 0);
        }

        //! @brief Retreving the sample image of the environment map.
        [[nodiscard]] vk::sample_image image() const { return m_skybox_image; }

        void destroy() {

            m_skybox_image.destruct();
            if (m_skybox_pipeline.alive()) {
                m_skybox_pipeline.destruct();
            }
            m_skybox_descriptors.destruct();
            m_skybox_ubo.destruct();
            m_skybox_shaders.destruct();
            m_skybox_vbo.destruct();
        }

        //! TODO: Logic for converting the HDR image handles to a skybox
        //! samplerCube
        void process_to_cubemap() {}

    private:
        VkDevice m_device = nullptr;

        vk::sample_image m_skybox_image;

        vk::shader_resource m_skybox_shaders{};
        vk::buffer m_skybox_ubo{};
        vk::descriptor_resource m_skybox_descriptors{};
        vk::pipeline m_skybox_pipeline{};
        vk::vertex_buffer m_skybox_vbo;
    };

};