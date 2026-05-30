module;

#include <string>
#include <vector>
#include <string_view>
#include <span>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/hash.hpp>
#include <tiny_obj_loader.h>

export module atlas.drivers.importer:obj_loader;

import vk;

template<typename T, typename... Rest>
void
hash_combine(size_t& seed, const T& v, const Rest&... rest) {
    seed ^= std::hash<T>()(v) + 0x9e3779b9 + (seed << 6) + (seed << 2);
    (hash_combine(seed, rest), ...);
}

namespace std {

    template<>
    struct hash<vk::vertex_input> {
        size_t operator()(const vk::vertex_input& vertex) const {
            size_t seed = 0;
            hash_combine(
              seed, vertex.position, vertex.color, vertex.normals, vertex.uv);
            return seed;
        }
    };
}

export namespace atlas {
    class obj_importer {
    public:
        obj_importer(std::string_view p_path) {
            m_load = load(p_path);
        }

        bool load(std::string_view p_path) {
            tinyobj::attrib_t attrib;
            std::vector<tinyobj::shape_t> shapes;
            std::vector<tinyobj::material_t> materials;
            std::string warn, err;

            if(!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, p_path.data())) {
                return false;
            }

            std::unordered_map<vk::vertex_input, uint32_t> unique_vertices{};

            for(const auto& shape_face : shapes) {
                for(const auto& index : shape_face.mesh.indices) {
                    vk::vertex_input vertex{};

                    if(!unique_vertices.contains(vertex)) {
                        unique_vertices[vertex] = static_cast<uint32_t>(m_vertices.size());
                        m_vertices.push_back(vertex);
                    }

                    if(index.vertex_index >= 0) {
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

                    if (!unique_vertices.contains(vertex)) {
                        unique_vertices[vertex] =
                        static_cast<uint32_t>(m_vertices.size());
                        m_vertices.push_back(vertex);
                    }

                    m_indices.push_back(unique_vertices[vertex]);
                }
            }

            return true;
        }

        [[nodiscard]] bool is_load() const {
            return m_load;
        }

        //! @return the geometry vertices
        std::span<vk::vertex_input> vertices() {
            return m_vertices;
        }

        //! @return the geometry indices
        std::span<uint32_t> indices() {
            return m_indices;
        }

    private:
        std::vector<vk::vertex_input> m_vertices;
        std::vector<uint32_t> m_indices;
        bool m_load=false;
    };
};