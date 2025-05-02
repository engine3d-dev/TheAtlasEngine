#pragma once

#include <glm/glm.hpp>

namespace atlas::physics {

    enum class collider_shape : uint8_t {
        Box,
        Sphere,
        Capsule,
    };

    struct collider_body {
        collider_shape shape_type = collider_shape::Box;

        glm::vec3 half_extents = glm::vec3(0.5f);
        float radius = 0.5f;               
        float capsule_half_height = 0.5f;        

        uint32_t body_id = 0;
    };

}