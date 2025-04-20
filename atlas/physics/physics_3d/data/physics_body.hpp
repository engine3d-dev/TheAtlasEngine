#pragma once
#include <glm/glm.hpp>

namespace atlas::physics {

    struct physics_body {
        glm::vec3 linear_velocity = glm::vec3(0.0);
        glm::vec3 angular_velocity = glm::vec3(0.0f);

        glm::vec3 cumulative_force = glm::vec3(0.0);
        glm::vec3 cumulative_torque = glm::vec3(0.0);

        float mass = 1.0f;
        float linear_damping = 0.0f;
        float angular_damping = 0.0f;

        glm::vec3 inertia = glm::vec3(1.0f);
        glm::vec3 inverse_inertia = glm::vec3(1.0f);

        bool use_gravity = true;
        float gravity_factor = 1.0f;
        bool collision_enabled = true;

        uint8_t body_type = 0;
        uint32_t friction = 1.0f;
        uint32_t restitution = 1.0f;

        glm::uvec3 linear_constraints = glm::uvec3(1);
        glm::uvec3 angular_constraints = glm::uvec3(1);

        uint32_t body_id = 0;
    };
};