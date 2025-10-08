#include <drivers/vulkan-cpp/mesh.hpp>
#include <tiny_obj_loader.h>
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <core/engine_logger.hpp>
#include <drivers/vulkan-cpp/hash.hpp>
#include <drivers/vulkan-cpp/vk_context.hpp>

namespace atlas::vk {
    mesh::mesh(std::span<::vk::vertex_input> p_vertices,
               std::span<uint32_t> p_indices) {
		m_physical = vk_context::physical_driver();
		m_device = vk_context::driver_context();
		::vk::vertex_buffer_settings vbo_settings = {
			.phsyical_memory_properties = m_physical.memory_properties(),
			.vertices = p_vertices
		};
		::vk::index_buffer_settings ibo_settings = {
			.phsyical_memory_properties = m_physical.memory_properties(),
			.indices = p_indices
		};
        m_vbo = ::vk::vertex_buffer(m_device, vbo_settings);
        m_ibo = ::vk::index_buffer(m_device, ibo_settings);
    }

    mesh::mesh(const std::filesystem::path& p_filename) {
		m_physical = vk_context::physical_driver();
        m_device = vk_context::driver_context();
		reload_mesh(p_filename);
    }

    void mesh::reload_mesh(const std::filesystem::path& p_filename) {
		console_log_info("Loading .obj file!!!");
		load_obj(p_filename);
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

        std::vector<::vk::vertex_input> vertices;
        std::vector<uint32_t> indices;
        std::unordered_map<::vk::vertex_input, uint32_t> unique_vertices{};

        for (const auto& shape : shapes) {
            for (const auto& index : shape.mesh.indices) {
                ::vk::vertex_input vertex{};

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

		::vk::vertex_buffer_settings vbo_settings = {
			.phsyical_memory_properties = m_physical.memory_properties(),
			.vertices = vertices
		};
		::vk::index_buffer_settings ibo_settings = {
			.phsyical_memory_properties = m_physical.memory_properties(),
			.indices = indices
		};
        m_vbo = ::vk::vertex_buffer(m_device, vbo_settings);
        m_ibo = ::vk::index_buffer(m_device, ibo_settings);
        m_model_loaded = true;
    }

    void mesh::initialize_uniforms(uint32_t p_size_bytes_ubo) {
		::vk::uniform_buffer_info geo_info = {
			.phsyical_memory_properties = m_physical.memory_properties(),
			.size_bytes = p_size_bytes_ubo
		};
        m_geoemtry_ubo = ::vk::uniform_buffer(m_device, geo_info);
    }

    void mesh::update_uniform(const material_uniform& p_material_ubo) {
        m_geoemtry_ubo.update(&p_material_ubo);
    }

    void mesh::add_texture(const std::filesystem::path& p_path) {
		::vk::texture_info config_texture = {
			.phsyical_memory_properties = m_physical.memory_properties(),
			.filepath = p_path
		};
		m_texture = ::vk::texture(m_device, config_texture);

		if(!m_texture.loaded()) {
			console_log_info("Texture {} is NOT loaded!!!", p_path.string());
			return;
		}

    }

    void mesh::draw(const VkCommandBuffer& p_current) {
        m_vbo.bind(p_current);
        if (m_ibo.size() > 0) {
            m_ibo.bind(p_current);
			vkCmdDrawIndexed(p_current, m_ibo.size(), 1, 0, 0, 0);
        }
        else {
            vkCmdDraw(p_current, m_vbo.size(), 1, 0, 0);
        }
    }

    void mesh::destroy() {
        m_vbo.destroy();
        m_ibo.destroy();

		m_texture.destroy();
        m_geoemtry_ubo.destroy();
    }
};