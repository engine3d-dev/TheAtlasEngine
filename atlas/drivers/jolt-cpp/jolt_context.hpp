#pragma once
#include <core/core.hpp>
#include <physics/physics_context.hpp>
#include <drivers/jolt-cpp/jolt_components.hpp>
#include <physics/components.hpp>
#include <drivers/jolt-cpp/jolt_contact_listener.hpp>
#include <drivers/jolt-cpp/interface/jolt_broad_phase.hpp>

namespace atlas::physics {
    /**
     * @brief This class is made to be 1 of three api wrappers for jolt. Jolt
     * context is specifically for engine only use of the api. It wraps the
     * inititialization, the physics step and the clean up. Allow use not to
     * have to write batching algorithms for each time we want to change the
     * settings of jolt physics.
     *
     * @remark This is different that jolt_api, which is the user wrapper. It is
     * meant to contain all the effects that can happen during runtime. Where
     * most of the data can be called by the user. m_physics_system is the only
     * link between the two classes. Both of which use it for different
     * purposes. Jolt context to control the functionality of the entire systems
     * and the Jolt Api for user based control of the system and bodies.
     *
     * @remark The third api is collisions which has its own context for jolt.
     */
    class jolt_context : public physics_context {
    public:
        jolt_context() = default;
        jolt_context(const jolt_settings& p_settings);
        ~jolt_context() override = default;

    private:
        /**
         * @brief Removes all physics bodies and shapes from level_scene.
         *
         * @warning Similar to create this only works with level_scene as of
         * right now.
         *
         */
        void destroy_bodies() override;

        void prepare_and_finalize() override;

        /**
         * @brief Runs a given <contact added> the frame the contact was
         * created. Allows for all contacts to run after the physics has
         * finished as to stop thread collision issues.
         *
         */
        // void execute_collisions() override;

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