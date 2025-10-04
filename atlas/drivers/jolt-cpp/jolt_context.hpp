#pragma once
#include <core/core.hpp>
#include <physics/physics_context.hpp>
#include <drivers/jolt-cpp/jolt_components.hpp>
#include <physics/components.hpp>
#include <drivers/jolt-cpp/jolt_contact_listener.hpp>
#include <drivers/jolt-cpp/interface/jolt_broad_phase.hpp>

namespace atlas::physics {
    /**
     * jolt_context is the backend implementation of physics context
     *
     * Which refers to the jolt-specific implementation that should only have
     * access to jolt-specific types.
     *
     * Any of the external abstraction and interaction should well be defined by
     * the engine-specific parameters.
     */
    class jolt_context : public physics_context {
    public:
        jolt_context() = default;
        jolt_context(const jolt_settings& p_settings);
        ~jolt_context() override = default;

    private:
        /**
         * @brief Performs any specific cleanup needed by Jolt
         */
        void destroy_bodies() override;

        void prepare_and_finalize() override;

        void update_simulation(float p_delta_time) override;

    protected:
    
        void emplace_box_collider(uint32_t p_entity_id,
                                  const transform* p_transform,
                                  const physics_body* p_body,
                                  const box_collider* p_collider) override;

        void emplace_sphere_collider(
          uint32_t p_entity_id,
          const transform* p_transform,
          const physics_body* p_body,
          const sphere_collider* p_collider) override;

        void emplace_capsule_collider(
          uint32_t p_entity_id,
          const transform* p_transform,
          const physics_body* p_body,
          const capsule_collider* p_collider) override;

        // void set_position_rotation(flecs::entity p_entity, const
        // physics_body* p_body, const box_collider* p_collider, const
        // transform* p_transform) override;
        transform context_read_transform(uint32_t p_id) override;

        physics_body context_read_physics_body(uint32_t p_id) override;

    private:
        //! @note Must be defined before physics can be initialized otherwise
        //! jolt cannot be created properly.
        jolt_settings m_settings;

        /**
         * @brief Creates a static allocation of all data
         *
         */
        ref<JPH::TempAllocatorImpl> m_temp_allocator;

        /**
         * @brief Sets up a thread system, either jolts or custom based on
         * thread settings in m_settings
         *
         */
        scope<JPH::JobSystemThreadPool> m_thread_system;

        /**
         * @brief Creates filtering for the quad tree in terms of movement
         *
         */
        ref<broad_phase_layer_interface> m_broad_phase_layer_interface;

        /**
         * @brief Creates a filter for the quad tree in terms of objects types
         *
         */
        ref<object_vs_broadphase_layer> m_object_vs_broadphase_filter;

        /**
         * @brief Creates a filter for pairs of collisions
         *
         */
        ref<object_layer_pair_filter> m_object_layer_pair_filter;

        /**
         * @brief Creates a way to recognize collisions
         *
         */
        // ref<contact_listener> m_contact_listener;
        contact_listener m_contact_listener;

        /**
         * @brief Gives access to the physics system. Is given to jolt_api as
         * well. This is to allow us to give seperation of concerns. However,
         * may change now do to Jolt conflict and unhandled dangling pointers
         * within jolt_api.
         *
         */
        ref<JPH::PhysicsSystem> m_physics_system;

        std::map<uint32_t, JPH::BodyID> m_cached_body_ids;
    };
};