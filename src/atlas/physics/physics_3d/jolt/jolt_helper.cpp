#include <glm/gtc/quaternion.hpp>
#include <physics/physics_3d/jolt/jolt_helper.hpp>

namespace atlas::physics {
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