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

    /**
     * @param non_moving is used for static objects that saves for not using the
     * collider component
     * @param moving is used for dynamic, kinematic, and character objects that
     * will be used
     */
    enum body_layer : uint8_t {
        non_moving = 0,
        moving = 1,
        layer_num,
    };

    enum activation : uint8_t { activate, deactivate };

    /**
     * @brief physics body data-driven representative
     *
     * TODO: Add parameters for force, impulse, and torque
     */
    struct physics_body {
        glm::vec3 linear_velocity = glm::vec3(0.0);
        glm::vec3 angular_velocity = glm::vec3(0.0f);

        glm::vec3 cumulative_force = glm::vec3(0.0f);
        glm::vec3 cumulative_torque = glm::vec3(0.0f);

        float mass_factor = 1.0f;
        glm::vec3 center_mass_position = glm::vec3(0.0f);
        float linear_damping = 0.0f;
        float angular_damping = 0.0f;

        float gravity_factor = 1.0f;
        float friction = 0.8f;
        float restitution = 0.2f;

        //! @brief body_type::fixed means this physics body is static
        uint8_t body_movement_type = body_type::fixed;

        //! @brief body_layer (object layers) refer to the rules of the
        //! collision system specified in JoltPhysics
        uint8_t body_layer_type = body_layer::moving;
    };

    struct box_collider {
        glm::vec3 half_extent = glm::vec3(0.5f);
    };

    struct capsule_collider {
        float radius = 0.5f;
        float half_height = 0.5f;
    };

    struct sphere_collider {
        float radius = 0.5f;
    };

};