#include <core/engine_logger.hpp>
#include <core/geometry/mesh.hpp>
#include <drivers/vertex_buffer.hpp>
#include <tiny_obj_loader.h>

#include <core/utilities/hash.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/hash.hpp>

namespace std {
    template<>
    struct hash<atlas::vertex> {
        size_t operator()(const atlas::vertex& vertex) const {
            size_t seed = 0;
            atlas::hash_combine(seed,
                                vertex.Position,
                                vertex.Color,
                                vertex.Normals,
                                vertex.TexCoords);
            return seed;
        }
    };

};

namespace atlas {

    static bool g_loaded_successful = false;
    Mesh::Mesh(const ref<vertex_buffer>& p_vertex_buffer,
               const ref<index_buffer>& p_index_buffer) {
        m_vertex_buffer = p_vertex_buffer;
        m_index_buffer = p_index_buffer;
    }

    Mesh::Mesh(const std::string& p_filepath) {
        //! @note TODO: DO NOT KEEP IT THIS WAY.
        *this = load(p_filepath);
    }

    bool Mesh::is_loaded() const {
        return g_loaded_successful;
    }

    Mesh Mesh::load(const std::string& p_filepath) {
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
              &attrib, &shapes, &materials, &warn, &err, p_filepath.c_str())) {
            console_log_warn("Could not load model from path {}", p_filepath);
            g_loaded_successful = false;
            return Mesh();
        }
        else {
            console_log_info("Model Loaded = {}", p_filepath);
            g_loaded_successful = true;
        }

        std::vector<vertex> vertices;
        // std::vector<uint32_t> indices;
        std::vector<uint32_t> indices;
        std::unordered_map<vertex, uint32_t> unique_vertices{};

        for (const auto& shape : shapes) {
            for (const auto& index : shape.mesh.indices) {
                vertex vertex{};

                if (index.vertex_index >= 0) {
                    vertex.Position = {
                        attrib.vertices[3 * index.vertex_index + 0],
                        attrib.vertices[3 * index.vertex_index + 1],
                        attrib.vertices[3 * index.vertex_index + 2]
                    };

                    vertex.Color = {
                        attrib.colors[3 * index.vertex_index + 0],
                        attrib.colors[3 * index.vertex_index + 1],
                        attrib.colors[3 * index.vertex_index + 2]
                    };
                }

                if (index.normal_index >= 0) {
                    vertex.Normals = {
                        attrib.normals[3 * index.normal_index + 0],
                        attrib.normals[3 * index.normal_index + 1],
                        attrib.normals[3 * index.normal_index + 2]
                    };
                }

                if (index.texcoord_index >= 0) {
                    vertex.TexCoords = {
                        attrib.texcoords[2 * index.texcoord_index + 0],
                        attrib.texcoords[2 * index.texcoord_index + 1]
                    };
                }

                // vertices.push_back(vertex);
                if (unique_vertices.contains(vertex) == 0) {
                    unique_vertices[vertex] =
                      static_cast<uint32_t>(vertices.size());
                    vertices.push_back(vertex);
                }

                indices.push_back(unique_vertices[vertex]);
            }
        }
        ref<vertex_buffer> vb = vertex_buffer::create(vertices);
        ref<index_buffer> ib = index_buffer::create(indices);
        return Mesh(vb, ib);
    }
};