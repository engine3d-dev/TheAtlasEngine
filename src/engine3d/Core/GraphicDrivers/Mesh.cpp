#include "EngineLogger.hpp"
#include "GraphicDrivers/VertexBuffer.hpp"
#include <Core/GraphicDrivers/Mesh.hpp>
#include <tiny_obj_loader.h>

#include <Core/utils/types.hpp>

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
    Mesh::Mesh(const Ref<VertexBuffer>& p_Vb, const Ref<IndexBuffer>& p_Ib){
        m_Vertices = p_Vb;
        m_Indices = p_Ib;
    }

    Mesh Mesh::LoadModel(const std::string& p_Filename){
        tinyobj::attrib_t attrib;
        std::vector<tinyobj::shape_t> shapes;
        std::vector<tinyobj::material_t> materials;
        std::string warn, err;

        if(!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, p_Filename.c_str())){
            ConsoleLogWarn("Could not Load Mesh!! In Mesh::LoadModel!");
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

                    //! @note Setting the colors
                    auto color_idx = 3 * index.vertex_index + 2;
                    if(color_idx < attrib.colors.size()){
                        vertex.Color = {
                            attrib.colors[color_idx - 2],
                            attrib.colors[color_idx - 1],
                            attrib.colors[color_idx - 0]
                        };
                    }
                    else{
                        vertex.Color = {0.f, 0.f, 0.f};
                    }
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