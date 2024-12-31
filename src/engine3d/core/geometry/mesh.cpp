#include <core/engine_logger.hpp>
#include <drivers/vertex_buffer.hpp>
#include <core/geometry/mesh.hpp>
#include <tiny_obj_loader.h>

#include <core/utilities/hash.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/hash.hpp>

namespace std{
    template<>
    struct hash<engine3d::Vertex>{
        size_t operator()(const engine3d::Vertex& vertex) const {
			size_t seed = 0;
            engine3d::HashCombine(seed, vertex.Position, vertex.Color, vertex.Normals, vertex.Uv);
            return seed;
		}
	};

};

namespace engine3d{
    Mesh::Mesh(const Ref<VertexBuffer>& p_Vb, const Ref<IndexBuffer>& p_Ib) : m_IsLoadedSuccessful(true){
        m_Vertices = p_Vb;
        m_Indices = p_Ib;
    }

    Mesh::Mesh(const std::string& p_Filename){
        //! @note TODO: DO NOT KEEP IT THIS WAY.
        *this = LoadModel(p_Filename);
    }

    Mesh Mesh::LoadModel(const std::string& p_Filename){
        tinyobj::attrib_t attrib;
        std::vector<tinyobj::shape_t> shapes;
        std::vector<tinyobj::material_t> materials;
        std::string warn, err;

        //! @note If we return the constructor then we can check if the mesh loaded successfully
        //! @note We also receive hints if the loading is successful!
        //! @note Return default constructor automatically returns false means that mesh will return the boolean as false because it wasnt successful
        if(!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, p_Filename.c_str())){
            ConsoleLogWarn("Could not load model from path {}", p_Filename);
            return Mesh();
        }

        std::vector<Vertex> vertices;
        // std::vector<uint32_t> indices;
        std::vector<uint32_t> indices;
        std::unordered_map<Vertex, uint32_t> unique_vertices{};

        for(const auto& shape : shapes){
            for(const auto& index : shape.mesh.indices){
                Vertex vertex{};

                if(index.vertex_index >= 0){
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

                if(index.normal_index >= 0){
                    vertex.Normals = {
                        attrib.normals[3 * index.normal_index + 0],
                        attrib.normals[3 * index.normal_index + 1],
                        attrib.normals[3 * index.normal_index + 2]
                    };
                }

                if(index.texcoord_index >= 0){
                    vertex.Uv = {
                        attrib.texcoords[2 * index.texcoord_index + 0],
                        attrib.texcoords[2 * index.texcoord_index + 1]
                    };
                }


                // vertices.push_back(vertex);
                if(unique_vertices.contains(vertex) == 0){
                    unique_vertices[vertex] = static_cast<uint32_t>(vertices.size());
                    vertices.push_back(vertex);
                }

                indices.push_back(unique_vertices[vertex]);
            }
        }
        Ref<VertexBuffer> vb = VertexBuffer::Create(vertices);
        Ref<IndexBuffer> ib = IndexBuffer::Create(indices);
        return Mesh(vb, ib);
    }
};