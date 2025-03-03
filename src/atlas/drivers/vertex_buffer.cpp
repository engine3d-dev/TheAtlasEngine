#include <core/application.hpp>
#include <drivers/vertex_buffer.hpp>
#include <drivers/vulkan/vulkan_vertex_buffer.hpp>
#include <glm/glm.hpp>

namespace atlas {
    ref<vertex_buffer> vertex_buffer::create(
      const std::vector<vertex>& p_vertices) {
        switch (application::current_api()) {
            case API::VULKAN:
                return create_ref<vk::vk_vertex_buffer>(p_vertices);
            default:
                return nullptr;
        }

        return nullptr;
    }

    void vertex_buffer::bind(const VkCommandBuffer& p_command_buffers) {
        bind_vertex_buffer(p_command_buffers);
    }

    void vertex_buffer::draw(const VkCommandBuffer& p_command_buffers) {
        render_vertex_buffer(p_command_buffers);
    }

};