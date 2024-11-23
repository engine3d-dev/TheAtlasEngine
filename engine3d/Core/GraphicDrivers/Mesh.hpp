#pragma once

#include "GraphicDrivers/VertexBuffer.hpp"

namespace engine3d{
    
    class Mesh{
    public:
        Mesh(Ref<VertexBuffer> p_Vb);

        Ref<VertexBuffer>& GetVertices() { return m_Vertices; }

    private:
        Ref<VertexBuffer> m_Vertices;
    };

};