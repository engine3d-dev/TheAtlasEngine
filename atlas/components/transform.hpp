#pragma once

#include <glm/glm.hpp>

namespace atlas {

    struct transform_physics {
        glm::highp_vec3 position{ 0.f };
        glm::highp_vec4 quaterion_rotation{0.0f};
        glm::highp_vec3 rotation{ 0.f };
        glm::highp_vec3 scale{ 1.f };
        glm::highp_vec4 color{ 1.f };
    };
};