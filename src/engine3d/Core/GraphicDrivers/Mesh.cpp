#include <Core/GraphicDrivers/Mesh.hpp>


namespace engine3d{
    Mesh::Mesh(Ref<VertexBuffer> p_Vb){
        m_Vertices = p_Vb;
    }
};