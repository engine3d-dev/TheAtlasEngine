#include <Core/SceneManagment/Components/SPComps/Transform.hpp>
namespace engine3d {
    void Transform::OnIntegrate(){}

    Transform::Transform () : 
        m_Position(0.0f),
        m_QuaterionRot(0.0f),
        m_AxisRotation(0.0f),
        m_Scale(0.0f){}

    glm::lowp_vec3 Transform::GetLPPos()
    {
        glm::lowp_vec3 lp_Position;
        lp_Position.x = m_Position.x;
        lp_Position.y = m_Position.y;
        lp_Position.z = m_Position.z;
        return lp_Position;
    }

    glm::lowp_vec4 Transform::GetLPQuat() 
    {
        glm::lowp_vec4 lp_Quat;
        lp_Quat.x = m_QuaterionRot.x;
        lp_Quat.y = m_QuaterionRot.y;
        lp_Quat.z = m_QuaterionRot.z;
        lp_Quat.w = m_QuaterionRot.w;
        return lp_Quat;
    }

    glm::lowp_vec3 Transform::GetLPAxisRot() 
    {
        glm::lowp_vec3 lp_AxisRot;
        lp_AxisRot.x = m_AxisRotation.x;
        lp_AxisRot.y = m_AxisRotation.y;
        lp_AxisRot.z = m_AxisRotation.z;
        return lp_AxisRot;
    }

    glm::lowp_vec3 Transform::GetLPSclae() 
    {
        glm::lowp_vec3 lp_Scale;
        lp_Scale.x = m_Scale.x;
        lp_Scale.y = m_Scale.y;
        lp_Scale.z = m_Scale.z;
        return lp_Scale;
    }
};