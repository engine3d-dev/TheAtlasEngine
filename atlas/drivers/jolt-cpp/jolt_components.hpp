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

    /**
     * @brief Used to keep global data for player access and use.
     * Tells how physics bodies should act within a given scene by
     * default.
     */
    struct jolt_config {
        // Global gravity vector for all in scene
        glm::vec3 gravity = glm::vec3(0.0f, -9.80665f, 0.0f);

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
     * These parameters are also only specific to the construction-level, not
     * initiation level of the API's
     *
     * @remark Min and max world bounds are values that are artbitrary (in other
     * words limit the simulation space) as JoltPhysics has a limit on distance
     * for its limitation in simulation space
     */
    struct jolt_settings {

        uint32_t allocation_amount = 10 * 1024 * 1024;

        //! @brief Specifying which threading system to use for Jolt.
        thread_type thread_type = thread_type::default_system;

        uint32_t physics_threads =
          std::max(1u, std::thread::hardware_concurrency() - 2);

        uint32_t max_jobs_power = 10;
        uint32_t max_barriers = physics_threads * 16;
        bool enable_multithread = true;

        // Max memory size per scene
        uint32_t max_bodies = 16384;
        uint32_t max_body_pairs = 32768;
        uint32_t max_contact_constraints = 8192;
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