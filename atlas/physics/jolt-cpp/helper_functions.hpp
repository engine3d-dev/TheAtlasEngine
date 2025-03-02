#pragma once
#include <Jolt/Jolt.h>
#include <core/math/types.hpp>
#include <glm/fwd.hpp>

namespace atlas {
    template<>
    struct vector3<JPH::Vec3> {
        vector3() = default;

        vector3(const JPH::Vec3& p_other) {
            value = { p_other.GetX(), p_other.GetY(), p_other.GetZ() };
        }

        operator glm::vec3() { return value; }

        glm::vec3 operator=(const JPH::Vec3& p_other) {
            return { p_other.GetX(), p_other.GetY(), p_other.GetZ() };
        }

        bool operator==(const glm::vec3& p_other) {
            return (value.x == p_other.x and value.y == p_other.y and
                    value.z == p_other.z);
        }

    private:
        glm::vec3 value;
    };
};

namespace atlas::physics {
    /**
     * @note So after dealing with this, we may need to figure out how we may
     * want to handle these
     */

    /* Converting from glm::vec3 to a jolt's vec3 math data type */
    JPH::RVec3 to_rvec3(const glm::vec3& p_Value);

    JPH::Vec3 to_vec3(const glm::vec3& p_Value);

    /* Converting jolt's vec3 back to a glm::vec3 */
    glm::vec3 to_vec3(const JPH::Vec3& p_Value);

    /* Converting Jolt's quaternion to glm's quaternion */
    glm::quat to_quat(const JPH::Quat& p_Value);

    JPH::Quat to_quat(glm::quat& p_Value);
};