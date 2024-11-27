#include <Core/SceneManagment/Components/SPComps/Transform.hpp>

namespace engine3d {
    void Transform::OnIntegrate(){}

    Transform::Transform () : 
        m_Position(0.0f),
        m_QuaterionRot(0.0f),
        m_AxisRotation(0.0f),
        m_Scale(0.0f){}
};