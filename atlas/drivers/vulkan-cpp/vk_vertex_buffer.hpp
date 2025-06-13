#pragma once
#include <span>
#include <drivers/vulkan-cpp/vk_types.hpp>

namespace atlas::vk {

    class vk_vertex_buffer {
    public:
        vk_vertex_buffer() = default;

        vk_vertex_buffer(const std::span<vertex>& p_vertices);

        ~vk_vertex_buffer() = default;

        void bind(const VkCommandBuffer& p_current);

        void draw(const VkCommandBuffer& p_current);

        [[nodiscard]] size_t size() const { return m_vertices_count; }

        [[nodiscard]] size_t size_bytes() const { return m_vertices_byte_size_count; }

        void destroy();

        operator VkBuffer() { return m_vertex_handler.handler; }

        operator VkBuffer() const { return m_vertex_handler.handler; }

    private:
        VkDevice m_driver=nullptr;
        uint32_t m_vertices_count=0;
        uint32_t m_vertices_byte_size_count=0;
        vk_buffer m_vertex_handler{};
    };
};