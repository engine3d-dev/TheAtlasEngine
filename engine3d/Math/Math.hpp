#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

namespace VectorConversion {
    template <typename Vector3>
        concept GLMVec3 = requires(Vector3 vector)
        {
            vector.x;
            vector.y;
            vector.z;
        };
    template <typename Vector4>
        concept GLMVec4 = GLMVec3<Vector4> && requires(Vector4 vector)
        {
            vector.w;
        };

        template <typename T,GLMVec3 U>
        T To_Type(const U &type)
        {
            return T(type.x, type.y, type.z);
        }

        template <typename T,GLMVec4 U>
        T To_Type(const U &type)
        {
            return T(type.x, type.y, type.z, type.w);
        }
}

namespace engine3d
{
    // glm::quat<glm::vec2> ToQuat()
    glm::vec4 ToQuat(glm::vec3 p_EulerAngles);
};