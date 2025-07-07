#include <drivers/vulkan-cpp/mesh.hpp>
#include <tiny_obj_loader.h>
// TinyGLTF includes (ensure you have these in your project)
#define TINYGLTF_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
// #include <core/image/stb_image.hpp>
#include <tiny_gltf.h>
#include <core/engine_logger.hpp>
#include <drivers/vulkan-cpp/hash.hpp>

namespace atlas::vk {
    mesh::mesh(const std::span<vertex_input>& p_vertices,
               const std::span<uint32_t>& p_indices) {
        m_vbo = vk_vertex_buffer(p_vertices);
        m_ibo = vk_index_buffer(p_indices);
    }

    mesh::mesh(const std::filesystem::path& p_filename) {
        reload_mesh(p_filename);
    }

    void mesh::reload_mesh(const std::filesystem::path& p_filename) {
        // if(p_filename.extension() == ".obj")
        // console_log_fatal("LOAD OBJ EXTENSION = {}",
        // p_filename.extension().string());
        if (p_filename.extension().string() == ".obj") {
            console_log_info("Loading .obj file!!!");
            load_obj(p_filename);
        }
        else if (p_filename.extension().string() == ".gltf") {
            console_log_info("Loading .gltf file!!!");
            load_gltf(p_filename);
        }
    }

    void mesh::load_gltf(
      [[maybe_unused]] const std::filesystem::path& p_filename) {
        std::vector<vertex_input> vertices;
        std::vector<uint32_t> indices;
        std::unordered_map<vertex_input, uint32_t> unique_vertices{};

        // Loading gltf
        tinygltf::Model model;
        tinygltf::TinyGLTF loader;
        std::string error_msg;
        std::string warn_msg;

        bool is_valid = loader.LoadASCIIFromFile(
          &model, &error_msg, &warn_msg, p_filename.string());

        if (!is_valid) {
            console_log_fatal(
              "Could not load gltf model from specified path {}",
              p_filename.string());
            if (!error_msg.empty()) {
                console_log_fatal("ERROR => {}", error_msg);
            }

            if (!warn_msg.empty()) {
                console_log_fatal("WARN => {}", warn_msg);
            }

            m_model_loaded = false;
            return;
        }

        const tinygltf::Scene scene_loading =
          model.scenes[model.defaultScene > -1 ? model.defaultScene : 0];

        for (size_t i = 0; i < scene_loading.nodes.size(); i++) {
            int node_idx = scene_loading.nodes[i];
            tinygltf::Node& node = model.nodes[node_idx];

            // If node.mesh == -1, do not process as it means there is no mesh
            if (node.mesh != -1) {
                const tinygltf::Mesh& mesh = model.meshes[node.mesh];

                for (const auto& primitive : mesh.primitives) {
                    // Only process triangles for now (mode 4)
                    if (primitive.mode != TINYGLTF_MODE_TRIANGLES) {
                        // std::cout << "Warning: Primitive mode " <<
                        // primitive.mode << " not supported. Skipping." <<
                        // std::endl;
                        continue;
                    }

                    // Get accessors for required attributes
                    const tinygltf::Accessor* pos_accessor = nullptr;
                    const tinygltf::Accessor* norm_accessor = nullptr;
                    const tinygltf::Accessor* uv_accessor = nullptr;
                    const tinygltf::Accessor* color_accessor = nullptr;
                    const tinygltf::Accessor* index_accessor = nullptr;

                    // Position
                    auto it_pos = primitive.attributes.find("POSITION");
                    if (it_pos != primitive.attributes.end()) {
                        pos_accessor = &model.accessors[it_pos->second];
                    }

                    // Normals (optional, but highly recommended)
                    auto it_norm = primitive.attributes.find("NORMAL");
                    if (it_norm != primitive.attributes.end()) {
                        norm_accessor = &model.accessors[it_norm->second];
                    }

                    // UVs (optional)
                    auto it_uv = primitive.attributes.find("TEXCOORD_0");
                    if (it_uv != primitive.attributes.end()) {
                        uv_accessor = &model.accessors[it_uv->second];
                    }

                    // Colors (optional, using COLOR_0)
                    auto it_color = primitive.attributes.find("COLOR_0");
                    if (it_color != primitive.attributes.end()) {
                        color_accessor = &model.accessors[it_color->second];
                    }

                    // Indices
                    if (primitive.indices != -1) {
                        index_accessor = &model.accessors[primitive.indices];
                    }

                    // --- Read data from buffers ---

                    // Get position buffer data
                    const tinygltf::BufferView& pos_buffer_view =
                      model.bufferViews[pos_accessor->bufferView];
                    const tinygltf::Buffer& pos_buffer =
                      model.buffers[pos_buffer_view.buffer];
                    const float* positions = reinterpret_cast<const float*>(
                      &pos_buffer.data[pos_buffer_view.byteOffset +
                                       pos_accessor->byteOffset]);
                    size_t pos_stride = pos_accessor->ByteStride(
                      pos_buffer_view); // Will be 12 for vec3f

                    // Get normal buffer data (if available)
                    const float* normals = nullptr;
                    size_t norm_stride = 0;
                    if (norm_accessor) {
                        const tinygltf::BufferView& norm_buffer_view =
                          model.bufferViews[norm_accessor->bufferView];
                        const tinygltf::Buffer& norm_buffer =
                          model.buffers[norm_buffer_view.buffer];
                        normals = reinterpret_cast<const float*>(
                          &norm_buffer.data[norm_buffer_view.byteOffset +
                                            norm_accessor->byteOffset]);
                        norm_stride =
                          norm_accessor->ByteStride(norm_buffer_view);
                    }

                    // Get UV buffer data (if available)
                    const float* uvs = nullptr;
                    size_t uv_stride = 0;
                    if (uv_accessor) {
                        const tinygltf::BufferView& uv_buffer_view =
                          model.bufferViews[uv_accessor->bufferView];
                        const tinygltf::Buffer& uv_buffer =
                          model.buffers[uv_buffer_view.buffer];
                        uvs = reinterpret_cast<const float*>(
                          &uv_buffer.data[uv_buffer_view.byteOffset +
                                          uv_accessor->byteOffset]);
                        uv_stride = uv_accessor->ByteStride(uv_buffer_view);
                    }

                    // Get Color buffer data (if available)
                    const float* colors =
                      nullptr; // Assuming float vec3 or vec4 for colors
                    size_t color_stride = 0;
                    if (color_accessor) {
                        const tinygltf::BufferView& color_buffer_view =
                          model.bufferViews[color_accessor->bufferView];
                        const tinygltf::Buffer& color_buffer =
                          model.buffers[color_buffer_view.buffer];
                        // Check component type for color (can be float, ushort,
                        // ubyte)
                        if (color_accessor->componentType ==
                            TINYGLTF_COMPONENT_TYPE_FLOAT) {
                            colors = reinterpret_cast<const float*>(
                              &color_buffer.data[color_buffer_view.byteOffset +
                                                 color_accessor->byteOffset]);
                        }
                        color_stride =
                          color_accessor->ByteStride(color_buffer_view);
                    }

                    // Get index buffer data
                    const tinygltf::BufferView& index_buffer_view =
                      model.bufferViews[index_accessor->bufferView];
                    const tinygltf::Buffer& index_buffer =
                      model.buffers[index_buffer_view.buffer];
                    const unsigned char* index_data =
                      &index_buffer.data[index_buffer_view.byteOffset +
                                         index_accessor->byteOffset];

                    // Determine index type and read count
                    size_t num_indices_to_read = index_accessor->count;
                    // size_t current_base_vertex_index = vertices.size(); // To
                    // remap glTF indices to our unique vertex indices

                    // --- Process each vertex using the original glTF indices
                    // ---
                    for (size_t k = 0; k < num_indices_to_read; ++k) {
                        uint32_t gltf_vertex_idx;

                        // Read the appropriate index type
                        switch (index_accessor->componentType) {
                            case TINYGLTF_COMPONENT_TYPE_UNSIGNED_BYTE: {
                                gltf_vertex_idx =
                                  static_cast<uint32_t>(index_data[k]);
                                break;
                            }
                            case TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT: {
                                gltf_vertex_idx = static_cast<uint32_t>(
                                  reinterpret_cast<const uint16_t*>(
                                    index_data)[k]);
                                break;
                            }
                            case TINYGLTF_COMPONENT_TYPE_UNSIGNED_INT: {
                                gltf_vertex_idx =
                                  reinterpret_cast<const uint32_t*>(
                                    index_data)[k];
                                break;
                            }
                        }

                        vertex_input current_vertex;

                        // Position (guaranteed to exist)
                        current_vertex.position = glm::vec3(
                          *(positions +
                            (gltf_vertex_idx * pos_stride / sizeof(float)) + 0),
                          *(positions +
                            (gltf_vertex_idx * pos_stride / sizeof(float)) + 1),
                          *(positions +
                            (gltf_vertex_idx * pos_stride / sizeof(float)) +
                            2));

                        // Normal
                        if (norm_accessor &&
                            norm_accessor->componentType ==
                              TINYGLTF_COMPONENT_TYPE_FLOAT &&
                            norm_accessor->type == TINYGLTF_TYPE_VEC3) {
                            current_vertex.normals =
                              glm::vec3(*(normals +
                                          (gltf_vertex_idx * norm_stride /
                                           sizeof(float)) +
                                          0),
                                        *(normals +
                                          (gltf_vertex_idx * norm_stride /
                                           sizeof(float)) +
                                          1),
                                        *(normals +
                                          (gltf_vertex_idx * norm_stride /
                                           sizeof(float)) +
                                          2));
                        }
                        else {
                            current_vertex.normals =
                              glm::vec3(0.0f, 0.0f, 0.0f); // Default normal
                        }

                        // UV
                        if (uv_accessor &&
                            uv_accessor->componentType ==
                              TINYGLTF_COMPONENT_TYPE_FLOAT &&
                            uv_accessor->type == TINYGLTF_TYPE_VEC2) {
                            current_vertex.uv = glm::vec2(
                              *(uvs +
                                (gltf_vertex_idx * uv_stride / sizeof(float)) +
                                0),
                              *(uvs +
                                (gltf_vertex_idx * uv_stride / sizeof(float)) +
                                1));
                        }
                        else {
                            current_vertex.uv =
                              glm::vec2(0.0f, 0.0f); // Default UV
                        }

                        // Color
                        if (color_accessor && color_accessor->componentType ==
                                                TINYGLTF_COMPONENT_TYPE_FLOAT) {
                            if (color_accessor->type == TINYGLTF_TYPE_VEC3) {
                                current_vertex.color =
                                  glm::vec3(*(colors +
                                              (gltf_vertex_idx * color_stride /
                                               sizeof(float)) +
                                              0),
                                            *(colors +
                                              (gltf_vertex_idx * color_stride /
                                               sizeof(float)) +
                                              1),
                                            *(colors +
                                              (gltf_vertex_idx * color_stride /
                                               sizeof(float)) +
                                              2));
                            }
                            if (color_accessor->type == TINYGLTF_TYPE_VEC4) {
                                // glTF colors are typically linear RGB. If you
                                // need sRGB, convert it.
                                current_vertex.color =
                                  glm::vec3(*(colors +
                                              (gltf_vertex_idx * color_stride /
                                               sizeof(float)) +
                                              0),
                                            *(colors +
                                              (gltf_vertex_idx * color_stride /
                                               sizeof(float)) +
                                              1),
                                            *(colors +
                                              (gltf_vertex_idx * color_stride /
                                               sizeof(float)) +
                                              2));
                                // Alpha component is at index 3 if needed:
                                // *(colors + ... + 3)
                            }
                            else {
                                current_vertex.color =
                                  glm::vec3(1.0f, 1.0f, 1.0f); // Default white
                            }
                        }
                        else {
                            current_vertex.color =
                              glm::vec3(1.0f, 1.0f, 1.0f); // Default white
                        }

                        // --- Deduplication ---
                        // If this vertex_input combination already exists, use
                        // its index. Otherwise, add it to our unique_vertices
                        // and out_vertices.
                        auto it = unique_vertices.find(current_vertex);
                        if (it != unique_vertices.end()) {
                            indices.push_back(it->second);
                        }
                        else {
                            // Add this unique vertex
                            uint32_t new_index =
                              static_cast<uint32_t>(vertices.size());
                            vertices.push_back(current_vertex);
                            unique_vertices[current_vertex] = new_index;
                            indices.push_back(new_index);
                        }
                    }
                }
            }
        }

        m_vbo = vk_vertex_buffer(vertices);
        m_ibo = vk_index_buffer(indices);
        m_model_loaded = true;
    }

    void mesh::load_obj(const std::filesystem::path& p_filename) {
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

        std::vector<vertex_input> vertices;
        std::vector<uint32_t> indices;
        std::unordered_map<vertex_input, uint32_t> unique_vertices{};

        for (const auto& shape : shapes) {
            for (const auto& index : shape.mesh.indices) {
                vertex_input vertex{};

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

                if (!unique_vertices.contains(vertex)) {
                    unique_vertices[vertex] =
                      static_cast<uint32_t>(vertices.size());
                    vertices.push_back(vertex);
                }

                indices.push_back(unique_vertices[vertex]);
            }
        }

        m_vbo = vk_vertex_buffer(vertices);
        m_ibo = vk_index_buffer(indices);
        m_model_loaded = true;
    }

    void mesh::initialize_uniforms(uint32_t p_size_bytes_ubo) {
        m_geoemtry_ubo = vk_uniform_buffer(p_size_bytes_ubo);
    }

    void mesh::update_uniform(const material_uniform& p_material_ubo) {
        m_geoemtry_ubo.update(&p_material_ubo);
    }

    void mesh::add_texture(const std::filesystem::path& p_path) {
        m_textures.emplace_back(p_path.string());
    }

    void mesh::draw(const VkCommandBuffer& p_current) {
        m_vbo.bind(p_current);
        if (m_ibo.size() > 0) {
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

        for (size_t i = 0; i < m_textures.size(); i++) {
            if (m_textures[i].loaded()) {
                m_textures[i].destroy();
            }
        }

        m_geoemtry_ubo.destroy();
    }
};