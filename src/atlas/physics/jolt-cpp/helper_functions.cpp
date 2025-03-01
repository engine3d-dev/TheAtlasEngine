#include <glm/gtc/quaternion.hpp>
#include <physics/jolt-cpp/helper_functions.hpp>


namespace atlas::physics{
    JPH::RVec3 to_rvec3(const glm::vec3& p_Value){
        return {p_Value.x, p_Value.y, p_Value.z};
    }

    JPH::Vec3 to_vec3(const glm::vec3& p_Value){
        return {p_Value.x, p_Value.y, p_Value.z};
    }

    glm::vec3 to_vec3(const JPH::Vec3& p_Value){
        return vector3<JPH::Vec3>(p_Value);
    }

    glm::quat to_quat(const JPH::Quat& p_Value){
        return glm::quat(p_Value.GetW(), p_Value.GetX(), p_Value.GetY(), p_Value.GetZ());
    }

    JPH::Quat to_quat(glm::quat& p_Value){
        return JPH::Quat(p_Value.w, p_Value.x, p_Value.y, p_Value.z);
    }
};