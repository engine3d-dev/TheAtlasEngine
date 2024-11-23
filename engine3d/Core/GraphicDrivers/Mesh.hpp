#pragma once

#include <Core/Core.hpp>
#include <Core/GraphicDrivers/VertexBuffer.hpp>
#include <Core/GraphicDrivers/IndexBuffer.hpp>

namespace engine3d{
    
    class Mesh{
    public:
        Mesh() = default;
        Mesh(const Ref<VertexBuffer>& p_Vb, const Ref<IndexBuffer>& p_Ib);

        Ref<VertexBuffer>& GetVertices() { return m_Vertices; }

        Ref<IndexBuffer>& GetIndices() { return m_Indices; }

    private:
        Ref<VertexBuffer> m_Vertices;
        Ref<IndexBuffer> m_Indices;
    };

};