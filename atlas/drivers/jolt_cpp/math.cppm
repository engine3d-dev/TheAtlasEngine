module;

#include <Jolt/Jolt.h>
// jolt's math includes
#include <Jolt/Math/MathTypes.h>
#include <Jolt/Math/Real.h>
#include <Jolt/Math/Quat.h>

#include <glm/glm.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>
export module atlas.drivers.jolt_cpp:math;
import atlas.core.math;

export namespace atlas {
    template<>
    struct vector3<JPH::Vec3> {
        vector3() = default;

        vector3(const JPH::Vec3& p_other) {
            m_value = { p_other.GetX(), p_other.GetY(), p_other.GetZ() };
        }

        operator glm::vec3() { return m_value; }

        glm::vec3 operator=(const JPH::Vec3& p_other) {
            return { p_other.GetX(), p_other.GetY(), p_other.GetZ() };
        }

        bool operator==(const glm::vec3& p_other) {
            return (m_value.x == p_other.x and m_value.y == p_other.y and
                    m_value.z == p_other.z);
        }

    private:
        glm::vec3 m_value;
    };

    namespace jolt {
        JPH::RVec3 to_rvec3(const glm::vec3& p_values) {
            return { p_values.x, p_values.y, p_values.z };
        }

        JPH::Vec3 to_vec3(const glm::vec3& p_values) {
            return { p_values.x, p_values.y, p_values.z };
        }

        JPH::Quat to_quat(const glm::vec4& q) {
            return { q.x, q.y, q.z, q.w };
        }

        JPH::Quat to_quat(glm::quat& p_values) {
            return JPH::Quat(p_values.w, p_values.x, p_values.y, p_values.z);
        }
    };

    glm::quat to_quat(const JPH::Quat& p_values) {
        return glm::quat(
          p_values.GetW(), p_values.GetX(), p_values.GetY(), p_values.GetZ());
    }

    glm::vec3 to_vec3(const JPH::Vec3& p_values) {
        return vector3<JPH::Vec3>(p_values);
    }

    glm::vec4 to_vec4(const JPH::Quat& p_values) {
        return glm::vec4(
          p_values.GetX(), p_values.GetY(), p_values.GetZ(), p_values.GetW());
    }
};