#pragma once
#include <core/core.hpp>
#include <Jolt/Jolt.h>
#include <drivers/jolt-cpp/jolt_components.hpp>
#include <drivers/jolt-cpp/jolt-imports.hpp>
#include <physics/components.hpp>
#include <core/scene/components.hpp>
#include <flecs.h>
#include <core/event/event_bus.hpp>

namespace atlas::physics {
    /**
     * @brief The context is the way to interact with specific backend context
     * implementation such as JoltPhysics as our specific implementation-backend
     *
     * Provides API's to interact with the implemented physics context to run
     * its simulation
     *
     */
    class physics_context {
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

    //! @brief initializes the physics backend. SHOULD have an API associated
    //! with but for now, we assume we only have JoltPhysics as our only physics
    //! backend
    ref<physics_context> initialize_physics_context(
      const jolt_settings& p_settings,
      event::event_bus& p_bus);
};