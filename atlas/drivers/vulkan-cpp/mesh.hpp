#pragma once
#include <array>
#include <drivers/vulkan-cpp/vk_vertex_buffer.hpp>
#include <drivers/vulkan-cpp/vk_index_buffer.hpp>

#include <drivers/vulkan-cpp/vk_types.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/hash.hpp>
#include <string>
#include <drivers/vulkan-cpp/vk_texture.hpp>
#include <filesystem>

namespace atlas::vk {

    /**
     * @brief mesh class specifically defined with vulkan implementations for specific primitives
     * TODO: Whenever we load in a texture that will be laucnhed asyncronously
    */
    class mesh {
    public:
        mesh() = default;
        mesh(const std::span<vertex>& p_vertices, const std::span<uint32_t>& p_indices);
        mesh(const std::filesystem::path& p_filename);

        void draw(const VkCommandBuffer& p_current);

        void destroy();

        [[nodiscard]] vk_vertex_buffer get_vertex() const { return m_vbo; }

        [[nodiscard]] vk_index_buffer get_index() const { return m_ibo; }

        [[nodiscard]] texture get_texture(uint32_t p_index_slot) const { return m_texture_slots[p_index_slot]; }

        void set_texture(uint32_t p_index, const std::string& p_filename);

    private:
        std::array<texture, 4> m_texture_slots{};
        vk_vertex_buffer m_vbo{};
        vk_index_buffer m_ibo{};
    };
};
