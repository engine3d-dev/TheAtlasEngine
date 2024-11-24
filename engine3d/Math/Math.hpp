#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

namespace engine3d{
    // glm::quat<glm::vec2> ToQuat()
    glm::vec4 ToQuat(glm::vec3 p_EulerAngles);
};