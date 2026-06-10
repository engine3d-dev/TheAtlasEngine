module;

#include <array>
#include <cstdint>
#include <cstring>
#include <filesystem>
#include <vector>
#include <span>
#include <string>
#include <print>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/hash.hpp>
#include <span>

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

export module atlas.drivers.vulkan:environment_map;

import vk;

struct environment_uniforms {
    glm::mat4 proj_view;
};

struct environment_push_constant {
    uint64_t scene_environment_address=0;
};

export class environment_map {
public:
    environment_map() = default;
    environment_map(std::shared_ptr<vk::device> p_device,
                    vk::physical_device& p_physical,
                    const std::string& p_filename, /*NOLINT*/VkFormat p_color_format, VkFormat p_depth_format) : m_color_format(p_color_format), m_depth_format(p_depth_format) {
        /*NOLINT*/ m_device = p_device;
        m_physical = p_physical;
        m_memory_mask = m_physical->memory_properties(vk::memory_property::host_visible_bit | vk::memory_property::host_cached_bit);

        construct(p_filename);
    }

    void construct(const std::string& p_filename) {
        bool res = create_hdr(p_filename);

        if (!res) {
            std::println("Cannot load environment: {}", p_filename);
        }

        if (res) {
            std::println("Loaded succesfully: {}", p_filename);
        }
        create_pipelines();
    }

    bool create_hdr(const std::string& p_filename) {
        stbi_set_flip_vertically_on_load(true);
        int w, h, channels;
        float* pixels =
          stbi_loadf(p_filename.c_str(), &w, &h, &channels, STBI_rgb_alpha);

        if (!pixels) {
            return false;
        }

        const uint64_t width = static_cast<uint64_t>(w);
        const uint64_t height = static_cast<uint64_t>(h);

        VkFormat texture_format = VK_FORMAT_R32G32B32A32_SFLOAT;
        const uint64_t bytes_per_pixel_channel = 16; // float are 4 bytes
        const uint64_t total_size_bytes =
          static_cast<uint64_t>(width * height * bytes_per_pixel_channel);
        const uint64_t image_size = total_size_bytes;

        vk::buffer_parameters staging_params = {
            .memory_mask = m_memory_mask,
            .usage = vk::buffer_usage::transfer_src_bit,
        };

        vk::buffer staging_buffer =
          vk::buffer(*m_device, image_size, staging_params);

        // Creating image handle to storing the HDR
        vk::image_params skybox_params = {
            .extent = { .width = static_cast<uint32_t>(width), .height = static_cast<uint32_t>(height), },
            .format = texture_format,
            .memory_mask = m_memory_mask,
            // .property = vk::memory_property::device_local_bit,
            .aspect = vk::image_aspect_flags::color_bit,
            .usage = vk::image_usage::transfer_dst_bit |
                        vk::image_usage::sampled_bit,
            // .view_type = VK_IMAGE_VIEW_TYPE_CUBE,
        };
        m_skybox_image = vk::sample_image(*m_device, skybox_params);

        // Transferring data from the CPU
        std::span<const uint8_t> pixels_data(
          reinterpret_cast<const uint8_t*>(pixels), image_size);
        staging_buffer.transfer(pixels_data);

        // Free CPU pixels immediately after staging copy
        stbi_image_free(pixels);

        // 6. Record and Execute Upload
        vk::command_params upload_params = {
            .levels = vk::command_levels::primary,
            .queue_index = 0, // Graphics Queue
            .flags = vk::command_pool_flags::reset,
        };
        vk::command_buffer upload_cmd(*m_device, upload_params);

        upload_cmd.begin(vk::command_usage::one_time_submit);

        // Begin Memory Barrier: Undefined to TRANSFER_DST
        m_skybox_image.memory_barrier(upload_cmd,
                                      texture_format,
                                      VK_IMAGE_LAYOUT_UNDEFINED,
                                      VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);

        vk::buffer_image_copy image_copy{
            .image_offset = { .width = 0, .height = 0, .depth = 0, },
            .image_extent = skybox_params.extent,
        };
        staging_buffer.copy_to_image(
          upload_cmd,
          m_skybox_image,
          std::span<const vk::buffer_image_copy>(&image_copy, 1));

        // Begin Memory Barrier: TRANSFER_DST to SHADER_READ_ONLY
        m_skybox_image.memory_barrier(upload_cmd,
                                      texture_format,
                                      VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                                      VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

        upload_cmd.end();

        VkQueue graphics_queue;
        vkGetDeviceQueue(*m_device, 0, 0, &graphics_queue);

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

        return true;
    }

    void create_buffers() {
        std::vector<vk::vertex_input> vertices = {
            // Front Face
            vk::vertex_input{
              .position = { -1.0f, 1.0f, -1.0f },
              .color = { 1.0f, 1.0f, 1.0f },
              .normals = { 0.0f, 0.0f, 0.0f },
              .uv = { 0.0f, 0.0f },
            },
            vk::vertex_input{
              .position = { -1.0f, -1.0f, -1.0f },
              .color = { 1.0f, 1.0f, 1.0f },
              .normals = { 0.0f, 0.0f, 0.0f },
              .uv = { 0.0f, 0.0f },
            },
            vk::vertex_input{
              .position = { 1.0f, -1.0f, -1.0f },
              .color = { 1.0f, 1.0f, 1.0f },
              .normals = { 0.0f, 0.0f, 0.0f },
              .uv = { 0.0f, 0.0f },
            },
            vk::vertex_input{
              .position = { 1.0f, -1.0f, -1.0f },
              .color = { 1.0f, 1.0f, 1.0f },
              .normals = { 0.0f, 0.0f, 0.0f },
              .uv = { 0.0f, 0.0f },
            },
            vk::vertex_input{
              .position = { 1.0f, 1.0f, -1.0f },
              .color = { 1.0f, 1.0f, 1.0f },
              .normals = { 0.0f, 0.0f, 0.0f },
              .uv = { 0.0f, 0.0f },
            },
            vk::vertex_input{
              .position = { -1.0f, 1.0f, -1.0f },
              .color = { 1.0f, 1.0f, 1.0f },
              .normals = { 0.0f, 0.0f, 0.0f },
              .uv = { 0.0f, 0.0f },
            },

            // Left Face
            vk::vertex_input{
              .position = { -1.0f, -1.0f, 1.0f },
              .color = { 1.0f, 1.0f, 1.0f },
              .normals = { 0.0f, 0.0f, 0.0f },
              .uv = { 0.0f, 0.0f },
            },
            vk::vertex_input{
              .position = { -1.0f, -1.0f, -1.0f },
              .color = { 1.0f, 1.0f, 1.0f },
              .normals = { 0.0f, 0.0f, 0.0f },
              .uv = { 0.0f, 0.0f },
            },
            vk::vertex_input{
              .position = { -1.0f, 1.0f, -1.0f },
              .color = { 1.0f, 1.0f, 1.0f },
              .normals = { 0.0f, 0.0f, 0.0f },
              .uv = { 0.0f, 0.0f },
            },
            vk::vertex_input{
              .position = { -1.0f, 1.0f, -1.0f },
              .color = { 1.0f, 1.0f, 1.0f },
              .normals = { 0.0f, 0.0f, 0.0f },
              .uv = { 0.0f, 0.0f },
            },
            vk::vertex_input{
              .position = { -1.0f, 1.0f, 1.0f },
              .color = { 1.0f, 1.0f, 1.0f },
              .normals = { 0.0f, 0.0f, 0.0f },
              .uv = { 0.0f, 0.0f },
            },
            vk::vertex_input{
              .position{ -1.0f, -1.0f, 1.0f },
              .color = { 1.0f, 1.0f, 1.0f },
              .normals = { 0.0f, 0.0f, 0.0f },
              .uv = { 0.0f, 0.0f },
            },

            // Right Face
            vk::vertex_input{
              .position = { 1.0f, -1.0f, -1.0f },
              .color = { 1.0f, 1.0f, 1.0f },
              .normals = { 0.0f, 0.0f, 0.0f },
              .uv = { 0.0f, 0.0f },
            },
            vk::vertex_input{
              .position = { 1.0f, -1.0f, 1.0f },
              .color = { 1.0f, 1.0f, 1.0f },
              .normals = { 0.0f, 0.0f, 0.0f },
              .uv = { 0.0f, 0.0f },
            },
            vk::vertex_input{
              .position = { 1.0f, 1.0f, 1.0f },
              .color = { 1.0f, 1.0f, 1.0f },
              .normals = { 0.0f, 0.0f, 0.0f },
              .uv = { 0.0f, 0.0f },
            },
            vk::vertex_input{
              .position = { 1.0f, 1.0f, 1.0f },
              .color = { 1.0f, 1.0f, 1.0f },
              .normals = { 0.0f, 0.0f, 0.0f },
              .uv = { 0.0f, 0.0f },
            },
            vk::vertex_input{
              .position = { 1.0f, 1.0f, -1.0f },
              .color = { 1.0f, 1.0f, 1.0f },
              .normals = { 0.0f, 0.0f, 0.0f },
              .uv = { 0.0f, 0.0f },
            },
            vk::vertex_input{
              .position = { 1.0f, -1.0f, -1.0f },
              .color = { 1.0f, 1.0f, 1.0f },
              .normals = { 0.0f, 0.0f, 0.0f },
              .uv = { 0.0f, 0.0f },
            },

            // Back Face
            vk::vertex_input{
              .position = { -1.0f, -1.0f, 1.0f },
              .color = { 1.0f, 1.0f, 1.0f },
              .normals = { 0.0f, 0.0f, 0.0f },
              .uv = { 0.0f, 0.0f },
            },
            vk::vertex_input{
              .position = { -1.0f, 1.0f, 1.0f },
              .color = { 1.0f, 1.0f, 1.0f },
              .normals = { 0.0f, 0.0f, 0.0f },
              .uv = { 0.0f, 0.0f },
            },
            vk::vertex_input{
              .position = { 1.0f, 1.0f, 1.0f },
              .color = { 1.0f, 1.0f, 1.0f },
              .normals = { 0.0f, 0.0f, 0.0f },
              .uv = { 0.0f, 0.0f },
            },
            vk::vertex_input{
              .position = { 1.0f, 1.0f, 1.0f },
              .color = { 1.0f, 1.0f, 1.0f },
              .normals = { 0.0f, 0.0f, 0.0f },
              .uv = { 0.0f, 0.0f },
            },
            vk::vertex_input{
              .position = { 1.0f, -1.0f, 1.0f },
              .color = { 1.0f, 1.0f, 1.0f },
              .normals = { 0.0f, 0.0f, 0.0f },
              .uv = { 0.0f, 0.0f },
            },
            vk::vertex_input{
              .position = { -1.0f, -1.0f, 1.0f },
              .color = { 1.0f, 1.0f, 1.0f },
              .normals = { 0.0f, 0.0f, 0.0f },
              .uv = { 0.0f, 0.0f },
            },

            // Top Face
            vk::vertex_input{
              .position = { -1.0f, 1.0f, -1.0f },
              .color = { 1.0f, 1.0f, 1.0f },
              .normals = { 0.0f, 0.0f, 0.0f },
              .uv = { 0.0f, 0.0f },
            },
            vk::vertex_input{
              .position = { 1.0f, 1.0f, -1.0f },
              .color = { 1.0f, 1.0f, 1.0f },
              .normals = { 0.0f, 0.0f, 0.0f },
              .uv = { 0.0f, 0.0f },
            },
            vk::vertex_input{
              .position = { 1.0f, 1.0f, 1.0f },
              .color = { 1.0f, 1.0f, 1.0f },
              .normals = { 0.0f, 0.0f, 0.0f },
              .uv = { 0.0f, 0.0f },
            },
            vk::vertex_input{
              .position = { 1.0f, 1.0f, 1.0f },
              .color = { 1.0f, 1.0f, 1.0f },
              .normals = { 0.0f, 0.0f, 0.0f },
              .uv = { 0.0f, 0.0f },
            },
            vk::vertex_input{
              .position = { -1.0f, 1.0f, 1.0f },
              .color = { 1.0f, 1.0f, 1.0f },
              .normals = { 0.0f, 0.0f, 0.0f },
              .uv = { 0.0f, 0.0f },
            },
            vk::vertex_input{
              .position = { -1.0f, 1.0f, -1.0f },
              .color = { 1.0f, 1.0f, 1.0f },
              .normals = { 0.0f, 0.0f, 0.0f },
              .uv = { 0.0f, 0.0f },
            },

            // Bottom Face
            vk::vertex_input{
              .position = { -1.0f, -1.0f, -1.0f },
              .color = { 1.0f, 1.0f, 1.0f },
              .normals = { 0.0f, 0.0f, 0.0f },
              .uv = { 0.0f, 0.0f },
            },
            vk::vertex_input{
              .position = { -1.0f, -1.0f, 1.0f },
              .color = { 1.0f, 1.0f, 1.0f },
              .normals = { 0.0f, 0.0f, 0.0f },
              .uv = { 0.0f, 0.0f },
            },
            vk::vertex_input{
              .position = { 1.0f, -1.0f, -1.0f },
              .color = { 1.0f, 1.0f, 1.0f },
              .normals = { 0.0f, 0.0f, 0.0f },
              .uv = { 0.0f, 0.0f },
            },
            vk::vertex_input{
              .position = { 1.0f, -1.0f, -1.0f },
              .color = { 1.0f, 1.0f, 1.0f },
              .normals = { 0.0f, 0.0f, 0.0f },
              .uv = { 0.0f, 0.0f },
            },
            vk::vertex_input{
              .position = { -1.0f, -1.0f, 1.0f },
              .color = { 1.0f, 1.0f, 1.0f },
              .normals = { 0.0f, 0.0f, 0.0f },
              .uv = { 0.0f, 0.0f },
            },
            vk::vertex_input{
              .position = { 1.0f, -1.0f, 1.0f },
              .color = { 1.0f, 1.0f, 1.0f },
              .normals = { 0.0f, 0.0f, 0.0f },
              .uv = { 0.0f, 0.0f },
            }
        };

        m_vertices_size = vertices.size();

        vk::buffer_parameters vertex_params = {
            // .memory_mask = p_physical.memory_properties(
            //   vk::memory_property::device_local_bit |
            //   vk::memory_property::host_visible_bit),
            .memory_mask = m_memory_mask,
            .usage = vk::buffer_usage::transfer_dst_bit |
                     vk::buffer_usage::vertex_buffer_bit,
        };

        m_skybox_vbo = vk::vertex_buffer(*m_device, vertices, vertex_params);
    }


    void create_pipelines() {
        create_buffers();
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
        m_skybox_shaders = vk::shader_resource(*m_device, shader_info);
        m_skybox_shaders.vertex_attributes(attribute);


        // Specifying descriptors

        std::array<vk::descriptor_entry, 1> entries = {
            vk::descriptor_entry{
                // layout (set = 0, binding = 1) uniform sampler2D textures[];
                .type = vk::descriptor_type::combined_image_sampler,
                .binding_point = {
                    .binding = 1,
                    .stage = vk::shader_stage::fragment,
                },
                .descriptor_count = 1,
                .flags = vk::descriptor_bind_flags::partially_bound_bit |
                        vk::descriptor_bind_flags::update_after_bind,
            },
        };


        // layout(set = 0, ...)
        uint32_t max_descriptors = 1;
        vk::descriptor_layout set0_layout = {
            .slot = 0,
            .max_sets = 1,
            .entries = entries, // descriptor layout entries description
            .descriptor_counts = std::span<const uint32_t>(&max_descriptors, 1),
        };
        m_skybox_descriptor = vk::descriptor_resource(*m_device, set0_layout, vk::descriptor_layout_flags::update_after_bind_pool);


        // specifying push constant for pipelines
        std::array<vk::color_blend_attachment_state, 1> color_blend_attachments = {
            vk::color_blend_attachment_state{},
        };

        std::array<vk::dynamic_state, 2> dynamic_states = {
            vk::dynamic_state::viewport, vk::dynamic_state::scissor,
        };

        uint32_t vertex_mask = static_cast<uint32_t>(vk::shader_stage::vertex);
        uint32_t fragment_mask = static_cast<uint32_t>(vk::shader_stage::fragment);
        uint32_t stage_mask = vertex_mask | fragment_mask;
        m_stage = static_cast<vk::shader_stage>(stage_mask);
        vk::push_constant_range range = {
            .stage = m_stage,
            .offset = 0,
            .range = sizeof(environment_push_constant),
        };

        VkDescriptorSetLayout descriptor0_layout = m_skybox_descriptor.layout();
        uint32_t color_format = static_cast<uint32_t>(m_color_format);
        vk::pipeline_params skybox_pipeline_params = {
            .use_render_pipeline = true,
                .color_attachment_formats = std::span<const uint32_t>(&color_format, 1),
                .depth_format = static_cast<uint32_t>(m_depth_format),
                .stencil_format = static_cast<uint32_t>(m_depth_format),
                .renderpass = nullptr,
                .shader_modules = m_skybox_shaders.handles(),
                .vertex_attributes = m_skybox_shaders.vertex_attributes(),
                .vertex_bind_attributes = m_skybox_shaders.vertex_bind_attributes(),
                .descriptor_layouts = std::span<VkDescriptorSetLayout>(&descriptor0_layout, 1),
                .color_blend = {
                    .attachments = color_blend_attachments,
                },
                .depth_stencil_enabled = true,
                .depth_stencil = {
                    .depth_test_enable = true,
                    .depth_write_enable = false,
                    .depth_compare_op = vk::compare_op::less_or_equal,
                    .depth_bounds_test_enable = false,
                    .stencil_test_enable = false,
                },
                .dynamic_states = dynamic_states,
                .push_constants = std::span<const vk::push_constant_range>(&range, 1),
        };
        m_skybox_pipeline = vk::pipeline(*m_device, skybox_pipeline_params);

        vk::buffer_parameters uniform_params = {
            .memory_mask = m_physical->memory_properties(vk::memory_property::host_visible_bit | vk::memory_property::host_cached_bit),
            .usage = vk::buffer_usage::uniform_buffer_bit | vk::buffer_usage::shader_device_address_bit,
            .allocate_flags = vk::memory_allocate_flags::device_address_bit_khr,
        };

        m_skybox_uniforms = vk::dyn::buffer(*m_device, sizeof(environment_uniforms), uniform_params);

        vk::write_image environment_image = {
            .sampler = m_skybox_image.sampler(),
            .view = m_skybox_image.image_view(),
            .layout = vk::image_layout::shader_read_only_optimal,
        };
        std::array<vk::write_image_descriptor, 1> write_descriptors = {
            // layout(set = 0, binding = 1) sampler2D environment;
            vk::write_image_descriptor{
                .dst_binding = 1,
                .sample_images = std::span<const vk::write_image>(&environment_image, 1),
            },
        };

        m_skybox_descriptor.update({}, write_descriptors);
    }

    void set_current_command(vk::command_buffer& p_current) {
        m_current_command = &p_current;
    }


    void begin(const glm::mat4& p_proj, const glm::mat4& p_view) {
        m_proj_view = p_proj * glm::mat4(glm::mat3(p_view));

        m_skybox_pipeline.bind(*m_current_command);
        environment_uniforms scene_ubo = {
            .proj_view = m_proj_view,
        };

        m_skybox_uniforms.transfer<environment_uniforms>(std::span<environment_uniforms>(&scene_ubo, 1));

        const VkDescriptorSet skybox_descriptor = m_skybox_descriptor;
        m_current_command->bind_descriptors(m_skybox_pipeline.layout(), VK_PIPELINE_BIND_POINT_GRAPHICS, std::span<const VkDescriptorSet>(&skybox_descriptor, 1));
    }

    void end() {
        const uint64_t scene_environment = m_skybox_uniforms.get_device_address();

        environment_push_constant environment_push_const = {
            .scene_environment_address = scene_environment,
        };

        m_skybox_pipeline.push_constant<environment_push_constant>(*m_current_command, environment_push_const, m_stage, 0);

        // Binding our vertex buffers here
        const VkBuffer& skybox_vertex = m_skybox_vbo;
        uint64_t offset = 0;
        m_current_command->bind_vertex_buffers(std::span<const VkBuffer>(&skybox_vertex, 1), std::span<const uint64_t>(&offset, 1));

        vkCmdDraw(*m_current_command, m_vertices_size, 1, 0, 0);
    }

    [[nodiscard]] VkImageView image_view() const {
        return m_skybox_image.image_view();
    }

    [[nodiscard]] VkSampler sampler() const { return m_skybox_image.sampler(); }

    void destruct() {
        m_skybox_vbo.destruct();
        m_skybox_image.destruct();
        m_skybox_descriptor.destruct();
        m_skybox_shaders.destruct();
        m_skybox_pipeline.destruct();
    }

private:
    glm::mat4 m_proj_view;
    vk::command_buffer* m_current_command;
    uint64_t m_vertices_size=0;
    std::shared_ptr<vk::device> m_device;
    std::optional<vk::physical_device> m_physical;
    vk::vertex_buffer m_skybox_vbo;
    vk::sample_image m_skybox_image;
    uint32_t m_memory_mask;
    vk::shader_stage m_stage;

    VkFormat m_color_format;
    VkFormat m_depth_format;
    vk::dyn::buffer m_skybox_uniforms;
    vk::descriptor_resource m_skybox_descriptor;
    vk::shader_resource m_skybox_shaders;
    vk::pipeline m_skybox_pipeline;
};