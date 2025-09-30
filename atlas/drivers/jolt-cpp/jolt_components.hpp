#pragma once

#include <glm/glm.hpp>
#include <cstdint>
#include <thread>
#include <Jolt/Jolt.h>
#include <Jolt/Physics/PhysicsSystem.h>
#include <Jolt/Physics/Collision/ContactListener.h>

namespace atlas::physics {

    enum thread_type : uint8_t {
        default_system = 0,
        job_system = 1,
    };

    // enum body_layer : uint8_t { NonMoving = 0, Moving = 1, LayerNum };

    /**
     * @brief This is a replacement for an event system to handle collider.
     * It will be replaced by an event system once we have functional one up
     * and running.
     *
     * Its point is to associate entities with user made jolt collisions.
     *
     */
    struct collider_event {
        uint64_t id = 0;
    };
}

//! @brief created for future use of dynamic physics material
//! @remark FIXME: This needs to be associated with our own interpretations of
//! how friction should work. Or have a way for users to override these
//! functions. Do not delete for it will be needed in the near furture.
enum combine_friction : uint8_t {
    friction_default = 0,
    friction_max = 1,
    friction_min = 2,
    friction_num = 3,
};

//! @brief Created for future use of dynamic physics material
//! @remark FIXME: Same thing here, this will be used in the near future. As it
//! is required by the basic jolt system. However, disccussion need to be made
//! about how we allow users to overwrite resititution.
enum combine_restitution : uint8_t {
    restitution_default = 0,
    restitution_max = 1,
    restitution_min = 2,
    restitution_num = 3,
};

namespace atlas::physics {

    /**
     * @brief Used to keep global data for player access and use.
     * Tells how physics bodies should act within a given scene by
     * default.
     */
    struct jolt_config {
        // Global gravity vector for all in scene
        glm::vec3 gravity = glm::vec3(0.0f, -9.80665f, 0.0f);

        // Friction Setting
        // This needs to be set to a function which makes it harder
        combine_friction friction_type = combine_friction::friction_default;

        // Restitution Settings
        // Same thing need functions for each.
        combine_restitution restitution_type =
          combine_restitution::restitution_default;

        //! @brief In seconds
        float time_before_sleep = 5.0f;

        // What 1 unit refers to in meters
        float world_unit_scale = 1.0f;

        // Helps stop the lauching of objects during numerical/flaoting point
        // errors when collision happen bertween to objects.
        float contact_bias_factor = 0.2f;
        float restitution_threshold = 1.0f;

        bool enable_constraints = true;
        bool enable_collision_callbacks = true;
    };

    /**
     * @brief Jolt-specific context configurations
     * These are going to be internally integrated to jolt_context
     * 
     * As these parameters currently are going to be specific to Jolt.
     * 
     * These parameters are also only specific to the construction-level, not initiation level of the API's
     * 
     * @remark Min and max world bounds are values that are artbitrary (in other words limit the simulation space)
     * as JoltPhysics has a limit on distance for its limitation in simulation space
    */
    struct jolt_settings {

        uint32_t allocation_amount = 10 * 1024 * 1024;

        // For job system
        thread_type thread_type = thread_type::default_system;

        uint32_t physics_threads =
          std::max(1u, std::thread::hardware_concurrency() - 2);

        uint32_t max_jobs_power = 10;
        uint32_t max_barriers = physics_threads * 16;
        // bool is_multithreaded = true;
        bool enable_multithread = true;

        // Filtering out specifica collisions
        // uint32_t collision_filter_layer_count = 32;
        // uint32_t default_filter_mask = 0xFFFFFFFF;

        // Scene position in the world
        // glm::vec3 physics_scene_position = glm::vec3(0.0f, 0.0f, 0.0f);

        // glm::vec3 world_bounds_min = glm::vec3(-500.0f, -500.0f, -500.0f);

        // glm::vec3 world_bounds_max = glm::vec3(500.0f, 500.0f, 500.0f);
        // Max memory size per scene
        uint32_t max_bodies = 16384;
        uint32_t max_body_pairs = 32768;
        uint32_t max_contact_constraints = 8192;

        // Time step for fps within the physics
        // If false set to 1/maxFps
        // float fixed_time_step = 1.0f / 60.0f;
        bool use_fixed_timestep = false;

        // solver for velocity and position
        // uint32_t position_iterations = 1;
        // uint32_t velocity_iterations = 8;

        // When to turn objects on and off depending on speed
        // float sleep_velocity_threshold = 0.05f;
        // float sleep_angular_velocity_threshold = 0.05f;
    };

    // This might be able to be generalized eventually but we will have to
    // create our own manifold before that happens.
    struct contact_event {
        uint64_t entity_a = 0;
        uint64_t entity_b = 0;
        JPH::ContactManifold manifold;
        JPH::ContactSettings settings;
    };

};