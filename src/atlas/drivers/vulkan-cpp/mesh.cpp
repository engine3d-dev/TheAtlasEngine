#include <drivers/vulkan-cpp/mesh.hpp>
#include <tiny_obj_loader.h>
#include <core/engine_logger.hpp>
#include <drivers/vulkan-cpp/hash.hpp>

namespace atlas::vk {
    mesh::mesh(const std::span<vertex>& p_vertices, const std::span<uint32_t>& p_indices) {
        m_vbo = vk_vertex_buffer(p_vertices);
        m_ibo = vk_index_buffer(p_indices);
    }

    mesh::mesh(const std::string& p_filename) {
        tinyobj::attrib_t attrib;
        std::vector<tinyobj::shape_t> shapes;
        std::vector<tinyobj::material_t> materials;
        std::string warn, err;

        //! @note If we return the constructor then we can check if the mesh
        //! loaded successfully
        //! @note We also receive hints if the loading is successful!
        //! @note Return default constructor automatically returns false means
        //! that mesh will return the boolean as false because it wasnt
        //! successful
        if (!tinyobj::LoadObj(
            &attrib, &shapes, &materials, &warn, &err, p_filename.c_str())) {
            console_log_warn("Could not load model from path {}", p_filename);
        }
        else {
            console_log_info("Model Loaded = {}", p_filename);
        }

        std::vector<vk::vertex> vertices;
        std::vector<uint32_t> indices;
        std::unordered_map<vertex, uint32_t> unique_vertices{};

        for (const auto& shape : shapes) {
            for (const auto& index : shape.mesh.indices) {
                vertex vertex{};

                // vertices.push_back(vertex);
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

                if (index.normal_index >= 0) {
                    vertex.normals = {
                        attrib.normals[3 * index.normal_index + 0],
                        attrib.normals[3 * index.normal_index + 1],
                        attrib.normals[3 * index.normal_index + 2]
                    };
                }

                if (index.texcoord_index >= 0) {
                    vertex.uv = {
                        attrib.texcoords[2 * index.texcoord_index + 0],
                    1.0f - attrib.texcoords[2 * index.texcoord_index + 1]
                    };
                }

                // vertices.push_back(vertex);
                if (!unique_vertices.contains(vertex)) {
                    unique_vertices[vertex] =
                      static_cast<uint32_t>(vertices.size());
                    vertices.push_back(vertex);
                }

                indices.push_back(unique_vertices[vertex]);
            }
        }

        console_log_fatal("vertices.size() = {}", vertices.size());
        console_log_fatal("indices.size() = {}", indices.size());

        m_vbo = vk_vertex_buffer(vertices);
        m_ibo = vk_index_buffer(indices);
    }

    void mesh::set_texture(uint32_t p_index, const std::string& p_filename) {
        m_texture_slots[p_index] = texture(p_filename);

        if(m_texture_slots[p_index].loaded()) {
            console_log_info("texture slot {} has successfully loaded file = {}", p_index, p_filename);
        }
    }

    void mesh::draw(const VkCommandBuffer& p_current) {
        m_vbo.bind(p_current);
        if(m_ibo.size() > 0) {
            m_ibo.bind(p_current);
            m_ibo.draw(p_current);
        }
        else {
            m_vbo.draw(p_current);
        }
    }

    void mesh::destroy() {
        m_vbo.destroy();
        m_ibo.destroy();

        for(size_t i = 0; i < m_texture_slots.size(); i++) {
            if(m_texture_slots[i].loaded()) {
                m_texture_slots[i].destroy();
            }
        }
    }
};