#pragma once

#include <glm/glm.hpp>

namespace atlas {

    // While integrating with the graphics make this transform instead of
    // transform physics
    //! @note This is the struct that jolt_context uses
    //! @note When changing to transform change jolt_context transform_physics
    //! to transform as well

    // This can be put in components.hpp as tranform after merging and fixing
    // renderer to have its own component for color and material.
    struct transform_physics {
        glm::highp_vec3 position{ 0.f };
        glm::highp_vec4 quaterion_rotation{ 0.0f };
        glm::highp_vec3 rotation{ 0.f };
        glm::highp_vec3 scale{ 1.f };
    };
};