module;

#include <cstddef>
#include <tiny_obj_loader.h>
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <span>
#include <filesystem>
#include <vulkan/vulkan.h>
#include <glm/glm.hpp>
#include <unordered_map>

export module atlas.drivers.vulkan.mesh;
import vk;

import atlas.logger;
import atlas.drivers.vulkan.instance_context;
import atlas.drivers.vulkan.physical_device;
import atlas.drivers.vulkan.device;
import atlas.drivers.vulkan.hash;
import atlas.drivers.vulkan.stb_image;

export namespace atlas::vulkan {
    /**
     * @brief mesh class specifically defined with vulkan implementations for
     * specific primitives
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
        mesh(std::span<vk::vertex_input> p_vertices,
             std::span<uint32_t> p_indices) {
            m_physical = instance_context::physical_driver();
            m_device = instance_context::logical_device();
            //! @brief Creating vertex/index buffers with host visibility flags
            const auto property_flags = static_cast<vk::memory_property>(
              vk::memory_property::host_visible_bit |
              vk::memory_property::host_cached_bit);

            vk::buffer_parameters vertex_params = {
                .memory_mask = m_physical.memory_properties(property_flags),
                .property_flags = vk::memory_property::device_local_bit,
                .usage = vk::buffer_usage::transfer_dst_bit |
                         vk::buffer_usage::vertex_buffer_bit,
            };

            vk::buffer_parameters index_params = {
                .memory_mask = m_physical.memory_properties(property_flags),
                .property_flags = static_cast<vk::memory_property>(
                  vk::memory_property::host_visible_bit |
                  vk::memory_property::host_cached_bit),
                .usage = vk::buffer_usage::index_buffer_bit,
            };

            m_vbo = vk::vertex_buffer(m_device, p_vertices, vertex_params);
            m_ibo = vk::index_buffer(m_device, p_indices, index_params);
        }

        mesh(const std::filesystem::path& p_filename, bool p_flip = false)
          : m_flip(p_flip) {
            m_physical = instance_context::physical_driver();
            m_device = instance_context::logical_device();
            reload_mesh(m_physical, p_filename);
        }

        //! @brief Reload mesh vertices and indices when requested
        void reload_mesh(const vk::physical_device& p_physical, const std::filesystem::path& p_filename) {
            load_obj(p_filename);
        }

        // void draw(const VkCommandBuffer& p_command_buffer) {
        //     m_vbo.bind(p_command_buffer);
        //     if (m_ibo.size() > 0) {
        //         m_ibo.bind(p_command_buffer);
        //         vkCmdDrawIndexed(p_command_buffer, m_ibo.size(), 1, 0, 0, 0);
        //     }
        //     else {
        //         vkCmdDraw(p_command_buffer, m_vbo.size(), 1, 0, 0);
        //     }
        // }

        [[nodiscard]] VkBuffer vertex_handle() const {
            return m_vbo;
        }

        [[nodiscard]] VkBuffer index_handle() const {
            return m_ibo;
        }

        void destroy() {
            m_vbo.destruct();
            m_ibo.destruct();

            m_diffuse.destruct();
            m_specular.destruct();
            m_geoemtry_ubo.destruct();
            m_material_ubo.destruct();
        }

        [[nodiscard]] bool has_indices() const {
            return m_has_indices;
        }

        //! @brief Loading single texture with specified std::filesystem::path
        void add_diffuse(const std::filesystem::path& p_path) {
            vk::texture_params config_texture = {
                .memory_mask = m_physical.memory_properties(
                  vk::memory_property::host_visible_bit |
                  vk::memory_property::host_cached_bit),
            };

            vk::stb_image image(p_path.string(), config_texture);
            m_diffuse = ::vk::texture(m_device, &image, config_texture);

            if (!m_diffuse.loaded()) {
                console_log_info("Diffuse Texture {} is NOT loaded!!!",
                                 p_path.string());
                return;
            }
        }

        void add_specular(const std::filesystem::path& p_path) {
            vk::texture_params config_texture = {
                .memory_mask = m_physical_device.memory_properties(
                  vk::memory_property::host_visible_bit |
                  vk::memory_property::host_cached_bit),
            };

            vk::stb_image image(p_path.string());
            m_specular = vk::texture(m_device, &image, config_texture);

            if (!m_specular.loaded()) {
                console_log_error("Specular Texture {} is NOT loaded!!!",
                                  p_path.string());
                return;
            }
        }

        [[nodiscard]] ::vk::sample_image diffuse() const {
            return m_diffuse.image();
        }
        [[nodiscard]] ::vk::sample_image specular() const {
            return m_specular.image();
        }

        //! @return true if mesh geometry model loaded succesfully
        [[nodiscard]] bool loaded() const { return m_model_loaded; }

        [[nodiscard]] bool diffuse_loaded() const { return m_diffuse.loaded(); }

        [[nodiscard]] bool specular_loaded() const {
            return m_specular.loaded();
        }

        void set_flip(bool p_flip) { m_flip = p_flip; }

    private:
        void load_obj(const std::filesystem::path& p_filename) {
            tinyobj::attrib_t attrib;
            std::vector<tinyobj::shape_t> shapes;
            std::vector<tinyobj::material_t> materials;
            std::string warn, err;

            //! @note If we return the constructor then we can check if the mesh
            //! loaded successfully
            //! @note We also receive hints if the loading is successful!
            //! @note Return default constructor automatically returns false
            //! means that mesh will return the boolean as false because it
            //! wasnt successful
            if (!tinyobj::LoadObj(&attrib,
                                  &shapes,
                                  &materials,
                                  &warn,
                                  &err,
                                  p_filename.string().c_str())) {
                console_log_warn("Could not load model from path {}",
                                 p_filename.string());
                m_model_loaded = false;
                return;
            }

            std::vector<vk::vertex_input> vertices;
            std::vector<uint32_t> indices;
            std::unordered_map<vk::vertex_input, uint32_t> unique_vertices{};

            // for (const auto& shape : shapes) {
            for (size_t i = 0; i < shapes.size(); i++) {
                auto shape = shapes[i];
                // for (const auto& index : shape.mesh.indices) {
                for (size_t j = 0; j < shape.mesh.indices.size(); j++) {
                    auto index = shape.mesh.indices[j];
                    vk::vertex_input vertex{};

                    if (!unique_vertices.contains(vertex)) {
                        unique_vertices[vertex] =
                          static_cast<uint32_t>(vertices.size());
                        vertices.push_back(vertex);
                    }

                    if (index.vertex_index >= 0) {
                        vertex.position = {
                            attrib.vertices[3 * index.vertex_index + 0],
                            attrib.vertices[3 * index.vertex_index + 1],
                            attrib.vertices[3 * index.vertex_index + 2]
                        };

                        vertex.color = {
                            attrib.colors[3 * index.vertex_index + 0],
                            attrib.colors[3 * index.vertex_index + 1],
                            attrib.colors[3 * index.vertex_index + 2]
                        };
                    }

                    if (!attrib.normals.empty()) {
                        vertex.normals = {
                            attrib.normals[3 * index.normal_index + 0],
                            attrib.normals[3 * index.normal_index + 1],
                            attrib.normals[3 * index.normal_index + 2]
                        };
                    }
                    if (!attrib.texcoords.empty()) {
                        glm::vec2 flipped_uv = {
                            attrib.texcoords[static_cast<long long>(
                                               index.texcoord_index) *
                                             2],
                            1.0f - attrib.texcoords[static_cast<long long>(
                                                      index.texcoord_index) *
                                                      2 +
                                                    1],
                        };

                        glm::vec2 original_uv = {
                            attrib.texcoords[static_cast<long long>(
                                               index.texcoord_index) *
                                             2],
                            attrib.texcoords[static_cast<long long>(
                                               index.texcoord_index) *
                                               2 +
                                             1],
                        };

                        vertex.uv = m_flip ? flipped_uv : original_uv;
                    }
                    else {
                        vertex.uv = glm::vec2(0.f, 0.f);
                    }

                    if (!unique_vertices.contains(vertex)) {
                        unique_vertices[vertex] =
                          static_cast<uint32_t>(vertices.size());
                        vertices.push_back(vertex);
                    }

                    indices.push_back(unique_vertices[vertex]);
                }
            }

            //! @brief Creating vertex/index buffers with host visibility flags
            const auto property_flags = static_cast<vk::memory_property>(
              vk::memory_property::host_visible_bit |
              vk::memory_property::host_cached_bit);

            vk::buffer_parameters vertex_params = {
                .memory_mask = p_physical.memory_properties(property_flags),
                .property_flags = vk::memory_property::device_local_bit,
                .usage = vk::buffer_usage::transfer_dst_bit |
                         vk::buffer_usage::vertex_buffer_bit,
            };

            vk::buffer_parameters index_params = {
                .memory_mask = p_physical.memory_properties(property_flags),
                .property_flags = static_cast<vk::memory_property>(
                  vk::memory_property::host_visible_bit |
                  vk::memory_property::host_cached_bit),
                .usage = vk::buffer_usage::index_buffer_bit,
            };
            m_vbo = ::vk::vertex_buffer(m_device, vertices, vertex_params);
            m_ibo = ::vk::index_buffer(m_device, indices, index_params);
            m_model_loaded = true;

            m_has_indices = (indices.size() > 0);
        }

    private:
        physical_device m_physical;
        VkDevice m_device = nullptr;
        vk::texture m_diffuse;
        vk::texture m_specular;
        vk::vertex_buffer m_vbo{};
        vk::index_buffer m_ibo{};
        vk::uniform_buffer m_geoemtry_ubo;
        vk::uniform_buffer m_material_ubo;
        bool m_model_loaded = false;
        bool m_flip = false;

        bool m_has_indices = 0;
    };
};