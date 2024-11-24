#include <Math/Math.hpp>

namespace engine3d{
    glm::vec4 ToQuat(glm::vec3 p_EulerAngles){
        glm::vec4 quaternion;
        glm::vec3 c = glm::cos(p_EulerAngles * float(0.5));
        glm::vec3 s = glm::sin(p_EulerAngles * float(0.5));

        quaternion.w = c.x * c.y * c.z + s.x * s.y * s.z;
        quaternion.x = s.x * c.y * c.z - c.x * s.y * s.z;
        quaternion.y = c.x * s.y * c.z + s.x * c.y * s.z;
        quaternion.z = c.x * c.y * s.z - s.x * s.y * c.z;

        return quaternion;
    }
};