#include "Core/SceneManagment/Components/GameComponent.hpp"
#include <glm/glm.hpp>
#include <Core/SceneManagment/SceneObjects/SceneObject.hpp>
class Transform : public engine3d::GameComponent
{
    public:
        Transform();
        glm::vec3 m_Position;
        glm::vec4 m_QuaterionRot;
        glm::vec3 m_AxisRotation;
        glm::vec3 m_Scale;


        glm::lowp_vec3 GetLPPos();
        glm::lowp_vec4 GetLPQuat();
        glm::lowp_vec3 GetLPAxisRot();
        glm::lowp_vec3 GetLPSclae();

};