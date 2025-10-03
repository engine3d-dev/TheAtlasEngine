#pragma once
#include <core/core.hpp>
#include <Jolt/Jolt.h>
#include <drivers/jolt-cpp/jolt_components.hpp>
#include <drivers/jolt-cpp/jolt-imports.hpp>
#include <physics/components.hpp>
#include <core/scene/components.hpp>
#include <flecs.h>

namespace atlas::physics {
    /**
     * @brief The context is the way to interact with only the engine. It is the
     * api for all the background funcitons and information that the user
     * shouldn not see. It is a virtual based type erasure class so thatmany
     * engines can implement the same functions. Specifically realted to backend
     * engine creation like batching, starting runtime, etc...
     *
     */
    class physics_context {
    public:
        virtual ~physics_context() = default;

        //! @brief Post-cleanup of any of the physics bodies parameters that has
        //! been created
        // for this runtime stimulation
        void destroy() { return destroy_bodies(); }

        //! @brief Executes any collision events that occurred
        // This is what the event system will use to detect any collided events
        // that occurred void update_collision_events() { return
        // execute_collisions(); }

        void update(float p_delta_time) {
            return update_simulation(p_delta_time);
        }

        //! @brief Prepare the bodies created with the colliders and finalizing
        //! those creations
        // into the physics system
        void prepare() { return prepare_and_finalize(); }

        void add_box_collider(uint32_t p_entity_id,
                              const transform* p_transform,
                              const physics_body* p_body,
                              const box_collider* p_collider) {
            return emplace_box_collider(
              p_entity_id, p_transform, p_body, p_collider);
        }

        void add_sphere_collider(uint32_t p_entity_id,
                                 const transform* p_transform,
                                 const physics_body* p_body,
                                 const sphere_collider* p_collider) {
            return emplace_sphere_collider(
              p_entity_id, p_transform, p_body, p_collider);
        }

        void add_capsule_collider(uint32_t p_entity_id,
                                  const transform* p_transform,
                                  const physics_body* p_body,
                                  const capsule_collider* p_collider) {
            return emplace_capsule_collider(
              p_entity_id, p_transform, p_body, p_collider);
        }

        transform read_transform(uint32_t p_id) {
            return context_read_transform(p_id);
        }

        physics_body read_physics_body(uint32_t p_id) {
            return context_read_physics_body(p_id);
        }

    private:
        virtual void destroy_bodies() = 0;

        // virtual void execute_collisions() = 0;

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
    };

    ref<physics_context> initialize_physics_context(
      const jolt_settings& p_settings);
};