#pragma once
#include <glm/glm.hpp>

namespace atlas {
    /**
     * @brief static is represented as fixed
     */
    enum body_type : uint8_t {
        fixed = 0,
        kinematic = 1,
        dynamic = 2,
        bodynum,
    };

    enum body_layer : uint8_t {
        non_moving = 0,
        moving = 1,
        layer_num,
    };

    enum activation : uint8_t {
        activate,
        deactivate
    };

    /**
     * @name Physics Body
     * @brief These are one time use calls. They are made to develop settings
     * for rigid bodies.
     *
     * @warning Some of these values should be read only. Meaning it might be
     * good to think about a read only version of imgui types.
     */
    struct physics_body {
        glm::vec3 linear_velocity = glm::vec3(0.0);
        glm::vec3 angular_velocity = glm::vec3(0.0f);

        glm::vec3 cumulative_force = glm::vec3(0.0);
        glm::vec3 cumulative_torque = glm::vec3(0.0);

        float mass_factor = 1.0f;
        glm::vec3 center_mass_position = glm::vec3(0.0);
        float linear_damping = 0.0f;
        float angular_damping = 0.0f;

        bool use_gravity = true;
        float gravity_factor = 1.0f;

        uint8_t body_type = 2;
        float friction = 0.8f;
        float restitution = 0.2f;

        uint8_t body_movement_type = body_type::fixed;
        uint8_t body_layer_type = body_layer::moving;
        activation mode = activation::deactivate;

        uint32_t body_id = 0;
        int count = 0;
    };

    struct box_collider {
        bool enable = true;
        uint8_t motion_type = body_type::fixed;
        uint8_t body_layer = body_layer::moving;
        glm::vec3 half_extent=glm::vec3(0.5f);
    };

    struct capsule_collider {
        bool enable = true;
        uint8_t motion_type = body_type::fixed;
        uint8_t body_layer = body_layer::moving;
        float radius=0.5f;
        float half_height = 0.5f;
    };

    struct sphere_collider {
        bool enable=true;
        uint8_t motion_type = body_type::fixed;
        uint8_t body_layer = body_layer::moving;
        float radius = 0.5f;
    };

};