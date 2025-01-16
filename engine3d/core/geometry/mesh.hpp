#pragma once
#include <core/core.hpp>
#include <drivers/vertex_buffer.hpp>
#include <drivers/index_buffer.hpp>

#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>

namespace engine3d{
    
    class Mesh{
    public:
        Mesh() = default;
        Mesh(const std::string& p_Path);
        Mesh(const Ref<VertexBuffer>& p_Vb, const Ref<IndexBuffer>& p_Ib);
        bool IsLoaded() const { return m_IsLoadedSuccessful; }
        static Mesh LoadModel(const std::string& p_Filename);

        Ref<VertexBuffer>& GetVertices() { return m_Vertices; }

        Ref<IndexBuffer>& GetIndices() { return m_Indices; }

    private:
        bool m_IsLoadedSuccessful = false;
        Ref<VertexBuffer> m_Vertices;
        Ref<IndexBuffer> m_Indices;
    };

};