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

        uint32_t body_id = 0;
        int count = 0;
    };

    /**
     *  @name Collider Shape
     *  @brief Required in order for collision types to be synced with jolts
     * shape filters
     */
    enum class collider_shape : uint8_t {
        box,
        sphere,
        capsule,
    };

    /**
     * @name Collider Body
     * @brief Some of this data is not required depending on the shape
     *
     * @remark If there is a way to hide or disable certains action in the
     * editor depending on the collider_shape, it would be good to do so in
     * this circumstance.
     */

    struct collider_body {
        bool collision_enabled = true;

        collider_shape shape_type = collider_shape::box;

        glm::vec3 half_extents = glm::vec3(0.5f);
        float radius = 0.5f;
        float capsule_half_height = 0.5f;

        // The id of the actuall JPH physics body
        uint32_t body_id = 0;
    };

};