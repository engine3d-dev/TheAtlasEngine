module;

#include <string>

#include <glm/glm.hpp>

#include <fastgltf/core.hpp>
#include <fastgltf/math.hpp>
#include <fastgltf/tools.hpp>
#include <fastgltf/types.hpp>
#include <fastgltf/glm_element_traits.hpp>

export module atlas.drivers.importer:gltf_importer;

import atlas.core.utilities;
import vk;

namespace atlas {
    export class gltf_importer {
    public:
        gltf_importer(const std::string& p_path, bool p_flip)
          : m_flip(p_flip) {
            m_load_successful = load(p_path);
        }

        bool load(const std::string& p_path) {
            if (!std::filesystem::exists(p_path)) {
                console_log_error("GLTF Error: Path does not exist: {}",
                                  p_path);
                return false;
            }

            size_t base_color_uv_index = 0;

            // 1. Create the Parser configuration (enable SIMD parsing if
            // available)
            fastgltf::Parser parser(fastgltf::Extensions::None);

            // 2. Map buffers directly without copying them into memory
            // prematurely
            constexpr auto gltf_options =
              fastgltf::Options::DontRequireValidAssetMember |
              fastgltf::Options::LoadExternalBuffers;

            std::filesystem::path filepath = std::filesystem::path(p_path);

            auto data = fastgltf::GltfDataBuffer::FromPath(filepath);
            if (data.error() != fastgltf::Error::None) {
                console_log_error("GLTF Error: Failed to load data buffer.");
                console_log_info("GLTF Error Message: {}",
                                 fastgltf::getErrorMessage(data.error()));
                m_load_successful = false;
                return m_load_successful;
            }

            auto asset =
              parser.loadGltf(data.get(), filepath.parent_path(), gltf_options);
            if (asset.error() != fastgltf::Error::None) {
                console_log_error("GLTF Error: Parsing failed with code {}",
                                  static_cast<size_t>(asset.error()));
                console_log_info("GLTF Error Message: {}",
                                 fastgltf::getErrorMessage(asset.error()));
                m_load_successful = false;
                return m_load_successful;
            }

            fastgltf::Asset& model = asset.get();

            // Position
            for (const auto& mesh : model.meshes) {
                for (const auto& primitive : mesh.primitives) {

                    // Get output primitive mesh texture color index
                    // (base_color_uv_index)
                    auto* primitive_mesh = primitive.findAttribute("POSITION");

                    if (primitive.materialIndex.has_value()) {
                        auto& material =
                          model.materials[primitive.materialIndex.value()];

                        auto& base_color_texture =
                          material.pbrData.baseColorTexture;

                        if (base_color_texture.has_value()) {
                            auto& texture =
                              model.textures[base_color_texture->textureIndex];

                            if (!texture.imageIndex.has_value()) {
                                return false;
                            }

                            if (base_color_texture->transform and
                                base_color_texture->transform->texCoordIndex
                                  .has_value()) {
                                base_color_uv_index =
                                  base_color_texture->transform->texCoordIndex
                                    .value();
                            }
                            else {
                                base_color_uv_index =
                                  material.pbrData.baseColorTexture
                                    ->texCoordIndex;
                            }
                        }
                    }

                    // Required to have a primitive that contains the meshes
                    // position
                    if (primitive_mesh == primitive.attributes.end()) {
                        m_load_successful = false;
                        return m_load_successful;
                    }

                    // Required to have indices associated with the mesh
                    if (!primitive.indicesAccessor.value()) {
                        m_load_successful = false;
                        return m_load_successful;
                    }

                    auto& position_accessor =
                      model.accessors[primitive_mesh->accessorIndex];

                    if (!position_accessor.bufferViewIndex.value()) {
                        m_load_successful = false;
                        return m_load_successful;
                    }

                    const auto& pos_accessor =
                      model.accessors[primitive_mesh->accessorIndex];
                    uint32_t base_vertex =
                      static_cast<uint32_t>(m_vertices.size());
                    m_vertices.reserve(base_vertex + pos_accessor.count);

                    // Check if this 3D model as texture coordinates or normals
                    auto uv_attribute = primitive.findAttribute(
                      std::format("TEXCOORD_{}", base_color_uv_index));
                    bool has_uv = (uv_attribute != primitive.attributes.end());

                    auto normal_attribute = primitive.findAttribute("NORMAL");
                    bool has_normals =
                      (normal_attribute != primitive.attributes.end());

                    fastgltf::iterateAccessorWithIndex<fastgltf::math::fvec3>(
                      model,
                      position_accessor,
                      [&](fastgltf::math::fvec3 pos, size_t index) {
                          vk::vertex_input vertex{};
                          vertex.position = { pos.x(), pos.y(), pos.z() };

                          if (has_normals) {
                              const auto& normal_accessor =
                                model
                                  .accessors[normal_attribute->accessorIndex];

                              fastgltf::math::fvec3 norm =
                                fastgltf::getAccessorElement<
                                  fastgltf::math::fvec3>(
                                  model, normal_accessor, index);
                              vertex.normals = { norm.x(), norm.y(), norm.z() };
                          }

                          if (has_uv) {
                              const auto& uv_accessor =
                                model.accessors[uv_attribute->accessorIndex];

                              // Extract 2D texture coordinate element matching
                              // based from the current primitive index
                              glm::vec2 uv =
                                fastgltf::getAccessorElement<glm::vec2>(
                                  model, uv_accessor, index);

                              glm::vec2 flipped_uv = { uv.x, 1.0f - uv.y };
                              vertex.uv = m_flip ? flipped_uv : uv;
                          }

                          m_vertices.push_back(vertex);
                      });

                    // Indices
                    if (primitive.indicesAccessor.has_value()) {
                        const auto& index_accessor =
                          model.accessors[primitive.indicesAccessor.value()];
                        m_indices.reserve(m_indices.size() +
                                          index_accessor.count);

                        fastgltf::iterateAccessor<uint32_t>(
                          model, index_accessor, [&](uint32_t index) {
                              m_indices.push_back(base_vertex + index);
                          });
                    }
                }
            }

            m_load_successful = true;
            return m_load_successful;
        }

        [[nodiscard]] bool load() const { return m_load_successful; }

        //! @return the geometry vertices
        std::span<vk::vertex_input> vertices() { return m_vertices; }

        //! @return the geometry indices
        std::span<uint32_t> indices() { return m_indices; }

    private:
        std::vector<vk::vertex_input> m_vertices;
        std::vector<uint32_t> m_indices;
        bool m_load_successful = false;
        bool m_flip = false;
    };
};