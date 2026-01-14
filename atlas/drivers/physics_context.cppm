module;

#include <glm/glm.hpp>
#include <type_traits>

#include <Jolt/Jolt.h>
#include <Jolt/Core/Core.h>
#include <Jolt/Physics/Body/BodyInterface.h>
#include <Jolt/Core/JobSystemThreadPool.h>
#include <Jolt/Physics/PhysicsSettings.h>
#include <Jolt/Core/IssueReporting.h>
#include <Jolt/Core/TempAllocator.h>
#include <Jolt/RegisterTypes.h>
#include <Jolt/Core/Factory.h>
#include <Jolt/Core/Memory.h>
#include <Jolt/Physics/Collision/ContactListener.h>
#include <Jolt/Physics/PhysicsSystem.h>
#include <Jolt/Physics/Collision/ObjectLayer.h>
#include <Jolt/Physics/Collision/BroadPhase/BroadPhaseLayer.h>
#include <Jolt/Core/IssueReporting.h>

#include <Jolt/Physics/Body/BodyCreationSettings.h>

#include <Jolt/Physics/Collision/Shape/Shape.h>
#include <Jolt/Physics/Collision/Shape/BoxShape.h>
#include <Jolt/Physics/Collision/Shape/ConvexShape.h>
#include <Jolt/Physics/Collision/Shape/CapsuleShape.h>
#include <Jolt/Physics/Collision/Shape/ScaledShape.h>
#include <Jolt/Physics/Collision/Shape/SphereShape.h>

#include <Jolt/Physics/EActivation.h>

// jolt's math includes
#include <Jolt/Math/MathTypes.h>
#include <Jolt/Math/Real.h>
#include <Jolt/Math/Quat.h>
#include <flecs.h>

export module atlas.drivers.physics_context;

import atlas.common;
import atlas.core.scene.components;
import atlas.core.event;



namespace atlas::physics {
    export enum thread_type : uint8_t {
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
    export struct jolt_settings {

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


    /**
     * @brief The context is the way to interact with specific backend context
     * implementation such as JoltPhysics as our specific implementation-backend
     *
     * Provides API's to interact with the implemented physics context to run
     * its simulation
     *
     */
    export class physics_context {
    public:
        virtual ~physics_context() = default;

        //! @brief Performs cleanup when simulation stops
        void destroy() { return destroy_bodies(); }

        //! @brief updates our simulation using delta time and works with a
        //! fixed timestep
        void update(float p_delta_time) {
            return update_simulation(p_delta_time);
        }

        /**
         * @brief As soon all physics bodies/colliders are created
         *
         * prepare() will be called to finalize all creation and apply them to
         * the physics system for simulation
         */
        void prepare() { return prepare_and_finalize(); }

        /**
         * @param p_entity_id is the entity ID to associate with adding the
         * sphere collider
         * @param p_transform provide the location of the entity when creating
         * this entity
         * @param p_physics_body provide the actual physics body representation
         * @param p_collider provides the actual sphere collider
         */
        void add_box_collider(uint32_t p_entity_id,
                              const transform* p_transform,
                              const physics_body* p_body,
                              const box_collider* p_collider) {
            return emplace_box_collider(
              p_entity_id, p_transform, p_body, p_collider);
        }

        /**
         * @param p_entity_id is the entity ID to associate with adding the
         * sphere collider
         * @param p_transform provide the location of the entity when creating
         * this entity
         * @param p_physics_body provide the actual physics body representation
         * @param p_collider provides the actual sphere collider
         */
        void add_sphere_collider(uint32_t p_entity_id,
                                 const transform* p_transform,
                                 const physics_body* p_body,
                                 const sphere_collider* p_collider) {
            return emplace_sphere_collider(
              p_entity_id, p_transform, p_body, p_collider);
        }

        /**
         * @param p_entity_id is the entity ID to associate with adding the
         * sphere collider
         * @param p_transform provide the location of the entity when creating
         * this entity
         * @param p_physics_body provide the actual physics body representation
         * @param p_collider provides the actual capsule collider
         */
        void add_capsule_collider(uint32_t p_entity_id,
                                  const transform* p_transform,
                                  const physics_body* p_body,
                                  const capsule_collider* p_collider) {
            return emplace_capsule_collider(
              p_entity_id, p_transform, p_body, p_collider);
        }

        /**
         * @param p_id is the entity ID that is required to specifiy which
         * entity this transform in physics simulation to return to
         * @return transform back to the entity after its modification
         */
        transform read_transform(uint32_t p_id) {
            return context_read_transform(p_id);
        }

        /**
         * @param p_id is the entity ID that is required to specifiy which
         * entity this transform in physics simulation to return to
         * @return physics body back to the entity after its modification
         */
        physics_body read_physics_body(uint32_t p_id) {
            return context_read_physics_body(p_id);
        }

        void set_linear_velocity(uint64_t p_id,
                                 const glm::vec3& p_linear_velocity) {
            return linear_velocity(p_id, p_linear_velocity);
        }

        void set_angular_velocity(uint64_t p_id,
                                  const glm::vec3& p_angular_velocity) {
            return angular_velocity(p_id, p_angular_velocity);
        }

        void set_force(uint64_t p_id, const glm::vec3& p_force) {
            return force(p_id, p_force);
        }

        void set_force_and_torque(uint64_t p_id,
                                  const glm::vec3& p_force,
                                  const glm::vec3& p_torque) {
            add_force_and_torque(p_id, p_force, p_torque);
        }

        void set_impulse(uint64_t p_id, const glm::vec3& p_impulse) {
            add_impulse(p_id, p_impulse);
        }

    private:
        virtual void destroy_bodies() = 0;

        /**
         * @brief Any emplace_* specific function are specific collider
         * implementation-specific to the backend (context) API's they are
         * implemented with
         *
         * Since colliders have specific parameters that define them. It would
         * simplify what parameter-access they have when adding these specific
         * colliders to the physics system
         */
        virtual void emplace_box_collider(uint32_t p_entity_id,
                                          const transform* p_transform,
                                          const physics_body* p_body,
                                          const box_collider* p_collider) = 0;

        virtual void emplace_sphere_collider(
          uint32_t p_entity_id,
          const transform* p_transform,
          const physics_body* p_body,
          const sphere_collider* p_collider) = 0;

        virtual void emplace_capsule_collider(
          uint32_t p_entity_id,
          const transform* p_transform,
          const physics_body* p_body,
          const capsule_collider* p_collider) = 0;

        virtual transform context_read_transform(uint32_t p_id) = 0;

        virtual physics_body context_read_physics_body(uint32_t p_id) = 0;

        virtual void prepare_and_finalize() = 0;

        virtual void update_simulation(float p_delta_time) = 0;

        virtual void linear_velocity(uint64_t p_id, const glm::vec3&) = 0;

        virtual void angular_velocity(uint64_t p_id, const glm::vec3&) = 0;

        virtual void force(uint64_t p_id,
                           const glm::vec3& p_cumulative_force) = 0;

        virtual void add_force_and_torque(uint64_t p_id,
                                          const glm::vec3& p_force,
                                          const glm::vec3& p_torque) = 0;

        virtual void add_impulse(uint64_t p_id, const glm::vec3& p_impulse) = 0;
    };
};