module;

#include <memory>
#include <print>
#include <array>
#include <span>

#include <vulkan/vulkan.h>
#include <flecs.h>

export module atlas.drivers.vulkan:render_context;
import atlas.core.scene;

import :graphics_context;
import vk;

export namespace atlas {

    struct mesh {
        uint32_t index_count=0;
        uint32_t instance=0;
        uint32_t first_index=0;
        uint32_t vertex_offset=0;
        uint32_t first_instance=0;
    };

    struct gpu_mesh_data {
        vk::buffer vertex;
        vk::buffer index;
    };

    /**
     * @brief Core render context to schedule images and barriers for coordinating rendering operations
     * 
     * Manages multiple render contexts
    */
    class render_context {
    public:
        render_context() = default;
        render_context(std::shared_ptr<graphics_context> p_context, VkFormat p_color_format, VkFormat p_depth_format) {
            m_device = p_context->logical_device();

            // Loading graphics pipeline
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
            // vk::shader_resource geometry_resource(logical_device, shader_info);
            m_shader_resource = vk::shader_resource(*m_device, shader_info);
            std::array<vk::color_blend_attachment_state, 1> color_blend_attachments = {
                vk::color_blend_attachment_state{},
            };

            std::array<vk::dynamic_state, 2> dynamic_states = {
                vk::dynamic_state::viewport, vk::dynamic_state::scissor
            };

            uint32_t format = static_cast<uint32_t>(p_color_format);
            vk::pipeline_params pipeline_configuration = {
                .use_render_pipeline = true,
                .color_attachment_formats = std::span<const uint32_t>(&format, 1),
                .depth_format = static_cast<uint32_t>(p_depth_format),
                .stencil_format = static_cast<uint32_t>(p_depth_format),
                .renderpass = nullptr,
                .shader_modules = m_shader_resource.handles(),
                .vertex_attributes = m_shader_resource.vertex_attributes(),
                .vertex_bind_attributes = m_shader_resource.vertex_bind_attributes(),
                .color_blend = {
                    .attachments = color_blend_attachments,
                },
                .depth_stencil_enabled = true,
                .dynamic_states = dynamic_states,
            };
            m_main_pipeline = vk::pipeline(*m_device, pipeline_configuration);
        }


        void set_command(vk::command_buffer& p_command) {
            m_current_command = &p_command;
        }


        void current_scene(flecs::world& p_world) {
            m_world = &p_world;
        }

        void bind_pipeline() {
            m_main_pipeline.bind(*m_current_command);

            vkCmdDraw(*m_current_command, 3, 1, 0, 0);
        }


        void destruct() {
            m_shader_resource.destruct();
            m_main_pipeline.destruct();
        }
    
    private:
        std::shared_ptr<vk::device> m_device;
        vk::command_buffer* m_current_command=nullptr;
        vk::shader_resource m_shader_resource;
        vk::pipeline m_main_pipeline;
        std::vector<VkDrawIndexedIndirectCommand> m_indirect_commands;
        std::vector<gpu_mesh_data> m_mesh_metadata;
        flecs::world* m_world=nullptr;
    };
};