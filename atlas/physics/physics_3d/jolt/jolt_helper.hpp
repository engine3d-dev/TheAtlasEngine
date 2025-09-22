#pragma once
#include <Jolt/Jolt.h>
#include <core/math/types.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/fwd.hpp>

namespace atlas {
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
};

namespace atlas::physics {

    inline JPH::Vec3 to_jph(const glm::vec3& v) {
        return { v.x, v.y, v.z };
    }

    inline JPH::Quat to_jph(const glm::vec4& q) {
        return { q.x, q.y, q.z, q.w };
    }
};