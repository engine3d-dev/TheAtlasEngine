#include <Jolt/Jolt.h>
#include <Core/SceneManagment/Components/GameComponent.hpp>
#include <Jolt/Math/Real.h>
#include <glm/detail/qualifier.hpp>
#include <glm/glm.hpp>
#include <glm/fwd.hpp>

namespace VectorConversion {
    template <typename Vector3>
        concept IsVec3 = requires(Vector3 vector)
        {
            vector.x;
            vector.y;
            vector.z;
        };

    template <typename Vector4>
        concept IsVec4 = IsVec3<Vector4> && requires(Vector4 vector)
        {
            vector.w;
        };

    template <typename Vector3>
        concept IsGetterVec3 = requires(Vector3 vector)
        {
            vector.GetX();
            vector.GetY();
            vector.GetZ();
        };

    template <typename Vector4>
        concept IsGetterVec4 = IsGetterVec3<Vector4> && requires(Vector4 vector)
        {
            vector.GetW();
        };
}


namespace engine3d 
{
    class Transform : public GameComponent
    {
        public:
            Transform();

            //Getters
            template<VectorConversion::IsVec3 T>
            T GetPos()
            {
                T position(m_Position.x,m_Position.y,m_Position.z);
                return position;
            }

            template<VectorConversion::IsVec4 Q>
            Q GetQuat()
            {
                Q quat(m_QuaterionRot.x,m_QuaterionRot.y,m_QuaterionRot.z,m_QuaterionRot.w);
                return quat;
            }

            template<VectorConversion::IsVec3 T>
            T GetAxisRot()
            {
                T axisRotation(m_AxisRotation.x,m_AxisRotation.y,m_AxisRotation.z);
                return axisRotation;
            }

            template<VectorConversion::IsVec3 T>
            T GetScale()
            {
                T scale(m_Scale.x,m_Scale.y,m_Scale.z);
                return scale;
            }

            //JPH Getters
            template<VectorConversion::IsGetterVec3 T>
            T GetPos()
            {
                T position(m_Position.x,m_Position.y,m_Position.z);
                return position;
            }

            template<VectorConversion::IsGetterVec4 Q>
            Q GetQuat()
            {
                Q quat(m_QuaterionRot.x,m_QuaterionRot.y,m_QuaterionRot.z,m_QuaterionRot.w);
                return quat;
            }

            template<VectorConversion::IsGetterVec3 T>
            T GetAxisRot()
            {
                T axisRotation(m_AxisRotation.x,m_AxisRotation.y,m_AxisRotation.z);
                return axisRotation;
            }

            template<VectorConversion::IsGetterVec3 T>
            T GetScale()
            {
                T scale(m_Scale.x,m_Scale.y,m_Scale.z);
                return scale;
            }

            //Setters
            template<VectorConversion::IsVec3 T>
            void SetPos(T position)
            {
                m_Position.x = position.x;
                m_Position.y = position.y;
                m_Position.z = position.z;
            }

            template<VectorConversion::IsVec4 Q>
            void SetQuat(Q quat)
            {
                m_QuaterionRot.x = quat.x;
                m_QuaterionRot.y = quat.y;
                m_QuaterionRot.z = quat.z;
                m_QuaterionRot.w = quat.w;
            }

            template<VectorConversion::IsVec3 T>
            void SetAxisRot(T axisRotation)
            {
                m_AxisRotation.x = axisRotation.x;
                m_AxisRotation.y = axisRotation.y;
                m_AxisRotation.z = axisRotation.z;
            }

            template<VectorConversion::IsVec3 T>
            void SetScale(T scale)
            {
                m_Scale.x = scale.x;
                m_Scale.y = scale.y;
                m_Scale.z = scale.z;
            }

            //Setters JPH
            template<VectorConversion::IsGetterVec3 T>
            void SetPos(T position)
            {
                m_Position.x = position.GetX();
                m_Position.y = position.GetY();
                m_Position.z = position.GetZ();
            }

            template<VectorConversion::IsGetterVec4 Q>
            void SetQuat(Q quat)
            {
                m_QuaterionRot.x = quat.GetX();
                m_QuaterionRot.y = quat.GetY();
                m_QuaterionRot.z = quat.GetZ();
                m_QuaterionRot.w = quat.GetW();
            }

            template<VectorConversion::IsGetterVec3 T>
            void SetAxisRot(T axisRotation)
            {
                m_AxisRotation.x = axisRotation.GetX();
                m_AxisRotation.y = axisRotation.GetY();
                m_AxisRotation.z = axisRotation.GetZ();
            }

            template<VectorConversion::IsGetterVec3 T>
            void SetScale(T scale)
            {
                m_Scale.x = scale.GetX();
                m_Scale.y = scale.GetY();
                m_Scale.z = scale.GetZ();
            }

            void OnIntegrate();

            glm::highp_vec3 m_Position;
            glm::highp_vec4 m_QuaterionRot;
            glm::highp_vec3 m_AxisRotation;
            glm::highp_vec3 m_Scale;

    };
};