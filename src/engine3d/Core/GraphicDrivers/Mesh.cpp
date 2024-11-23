#include <Core/GraphicDrivers/Mesh.hpp>


namespace engine3d{
    Mesh::Mesh(const Ref<VertexBuffer>& p_Vb, const Ref<IndexBuffer>& p_Ib){
        m_Vertices = p_Vb;
        m_Indices = p_Ib;
    }
};