#pragma once
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/hash.hpp>
#include <filesystem>
#include <vulkan-cpp/uniform_buffer.hpp>
#include <vulkan-cpp/vertex_buffer.hpp>
#include <vulkan-cpp/index_buffer.hpp>
#include <vulkan-cpp/texture.hpp>
#include <drivers/vulkan-cpp/vk_types.hpp>
#include <drivers/vulkan-cpp/vk_physical_driver.hpp>

namespace atlas::vk {

    /**
     * @brief mesh class specifically defined with vulkan implementations for
     * specific primitives
     * TODO: Whenever we load in a texture that will be laucnhed asyncronously
     *
     * @brief mesh class will contain metadata needed by vulkan specifications
     * Ways to communicate through vulkan by only supplying information needed
     * to update this mesh
     *
     * @brief Represents a renderable object -- supporting various material
     * types, etc
     * TODO - For now we have a map<name: string, material_source>, this should
     * be expanded to a proper material system for blending various materials
     */
    class mesh {
    public:
        mesh() = default;
        mesh(std::span<::vk::vertex_input> p_vertices,
             std::span<uint32_t> p_indices);
        mesh(const std::filesystem::path& p_filename);

        //! @brief Reload mesh vertices and indices when requested
        void reload_mesh(const std::filesystem::path& p_path);

        void initialize_uniforms(uint32_t p_size_bytes_ubo);

        void update_uniform(const material_uniform& p_material_ubo);

        [[nodiscard]] ::vk::uniform_buffer material_ubo() const {
            return m_geoemtry_ubo;
        }

        void draw(const VkCommandBuffer& p_command_buffer);

        void destroy();

        //! @brief Loading single texture with specified std::filesystem::path
        void add_diffuse(const std::filesystem::path& p_path);

        void add_specular(const std::filesystem::path& p_path);

        // [[nodiscard]] ::vk::sample_image image() const {
        //     return m_texture.image();
        // }

        [[nodiscard]] ::vk::sample_image diffuse() const { return m_diffuse.image(); }
        [[nodiscard]] ::vk::sample_image specular() const { return m_specular.image(); }

        //! @return true if mesh geometry model loaded succesfully
        [[nodiscard]] bool loaded() const { return m_model_loaded; }

        [[nodiscard]] bool diffuse_loaded() const { return m_diffuse.loaded(); }
        [[nodiscard]] bool specular_loaded() const { return m_specular.loaded(); }



    private:
        void load_obj(const std::filesystem::path& p_filename);

    private:
        vk_physical_driver m_physical;
        VkDevice m_device = nullptr;
        ::vk::texture m_diffuse;
        ::vk::texture m_specular;
        ::vk::vertex_buffer m_vbo{};
        ::vk::index_buffer m_ibo{};
        ::vk::uniform_buffer m_geoemtry_ubo;
        bool m_model_loaded = false;
    };
};
