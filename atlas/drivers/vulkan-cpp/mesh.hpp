#pragma once
#include <drivers/vulkan-cpp/vk_vertex_buffer.hpp>
#include <drivers/vulkan-cpp/vk_index_buffer.hpp>

#include <drivers/vulkan-cpp/vk_types.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/hash.hpp>
#include <drivers/vulkan-cpp/vk_texture.hpp>
#include <filesystem>
#include <drivers/vulkan-cpp/vk_uniform_buffer.hpp>

namespace atlas::vk {

    /**
     * @brief mesh class specifically defined with vulkan implementations for specific primitives
     * TODO: Whenever we load in a texture that will be laucnhed asyncronously
     * 
     * @brief mesh class will contain metadata needed by vulkan specifications
     * Ways to communicate through vulkan by only supplying information needed to update this mesh
     * 
     * @brief Represents a renderable object -- supporting various material types, etc
     * TODO - For now we have a map<name: string, material_source>, this should be expanded
     * to a proper material system for blending various materials
    */
    class mesh {
    public:
        mesh() = default;
        mesh(const std::span<vertex_input>& p_vertices, const std::span<uint32_t>& p_indices);
        mesh(const std::filesystem::path& p_filename);

        void initialize_uniforms(uint32_t p_size_bytes_ubo);

        void update_uniform(const material_uniform& p_material_ubo);

        [[nodiscard]] vk_uniform_buffer material_ubo() const { return m_geoemtry_ubo; }

        void draw(const VkCommandBuffer& p_command_buffer);

        void destroy();

        [[nodiscard]] vk_vertex_buffer get_vertex() const { return m_vbo; }

        [[nodiscard]] vk_index_buffer get_index() const { return m_ibo; }

        //! @brief Loading texture with specified filepath
        void add_texture(const std::filesystem::path& p_path);

        [[nodiscard]] std::span<texture> read_textures() {
            return m_textures;
        }

        [[nodiscard]] bool loaded() const { return m_model_loaded; }

    private:
        std::vector<texture> m_textures;
        vk_vertex_buffer m_vbo{};
        vk_index_buffer m_ibo{};
        vk_uniform_buffer m_geoemtry_ubo;
        bool m_model_loaded=false;
    };
};
