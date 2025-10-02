#include <drivers/jolt-cpp/types.hpp>

namespace atlas::physics {
    JPH::Vec3 to_jph(const glm::vec3& v) {
        return { v.x, v.y, v.z };
    }

    JPH::Quat to_jph(const glm::vec4& q) {
        return { q.x, q.y, q.z, q.w };
    }

    JPH::RVec3 to_rvec3(const glm::vec3& p_value) {
        return { p_value.x, p_value.y, p_value.z };
    }

    JPH::Vec3 to_vec3(const glm::vec3& p_value) {
        return { p_value.x, p_value.y, p_value.z };
    }

    glm::vec3 to_vec3(const JPH::Vec3& p_value) {
        return vector3<JPH::Vec3>(p_value);
    }

    glm::quat to_quat(const JPH::Quat& p_value) {
        return glm::quat(
          p_value.GetW(), p_value.GetX(), p_value.GetY(), p_value.GetZ());
    }

    JPH::Quat to_quat(glm::quat& p_value) {
        return JPH::Quat(p_value.w, p_value.x, p_value.y, p_value.z);
    }
};