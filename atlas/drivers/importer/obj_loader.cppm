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
        obj_importer(const std::string& p_path, bool p_flip)
          : m_flip(p_flip) {
            m_load = load(p_path);
        }

        bool load(const std::string& p_path) {
            tinyobj::attrib_t attrib;
            std::vector<tinyobj::shape_t> shapes;
            std::vector<tinyobj::material_t> materials;
            std::string warn;
            std::string err;

            if (!tinyobj::LoadObj(
                  &attrib, &shapes, &materials, &warn, &err, p_path.c_str())) {
                return false;
            }

            std::unordered_map<vk::vertex_input, uint32_t> unique_vertices{};

            for (const auto& shape_face : shapes) {
                for (const auto& index : shape_face.mesh.indices) {
                    vk::vertex_input vertex{};

                    if (!unique_vertices.contains(vertex)) {
                        unique_vertices[vertex] =
                          static_cast<uint32_t>(m_vertices.size());
                        m_vertices.push_back(vertex);
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
                          static_cast<uint32_t>(m_vertices.size());
                        m_vertices.push_back(vertex);
                    }

                    m_indices.push_back(unique_vertices[vertex]);
                }
            }

            return true;
        }

        [[nodiscard]] bool load() const { return m_load; }

        //! @return the geometry vertices
        std::span<vk::vertex_input> vertices() { return m_vertices; }

        //! @return the geometry indices
        std::span<uint32_t> indices() { return m_indices; }

    private:
        std::vector<vk::vertex_input> m_vertices;
        std::vector<uint32_t> m_indices;
        bool m_flip = false;
        bool m_load = false;
    };
};