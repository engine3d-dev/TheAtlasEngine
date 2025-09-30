#pragma once

#include <drivers/jolt-cpp/jolt_contact_listener.hpp>
#include <core/core.hpp>

#include <physics/physics_3d/physics_context.hpp>
#include <drivers/jolt-cpp/jolt_components.hpp>
#include <drivers/jolt-cpp/interface/jolt_broad_phase.hpp>
#include <physics/components.hpp>

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
        ~jolt_context() override;

    private:
        /**
         * @brief Creates all of the physics bodies at the start of runtime.
         * Utilizing the main scene provided in flecs to establish what to load.
         *
         * @warning Main scene is not defined and therefore will only utilzed
         * level_scene as of right now. In order to fix this issue a
         * get_main_scene() or get_main_scenes() must be created to develop a
         * more dynamic system.
         */
        void create_bodies() override;

        /**
         * @brief Removes all physics bodies and shapes from level_scene.
         *
         * @warning Similar to create this only works with level_scene as of
         * right now.
         *
         */
        void destroy_bodies() override;

        /**
         * @brief Runs a given <contact added> the frame the contact was
         * created. Allows for all contacts to run after the physics has
         * finished as to stop thread collision issues.
         *
         */
        void execute_collisions() override;

        ref<JPH::PhysicsSystem>& set_physics_instance() override { return m_physics_system; }
        void update_simulation(float p_delta_time) override;

        /**
         * @brief Creates a shape given a jolt collider
         *
         * @param e the entity who shape you are creating
         * @param collider the entities jolt collider
         * @return JPH::RefConst<JPH::Shape> The shape needs to be created and
         * stored for ref counting and scope.
         */
        JPH::RefConst<JPH::Shape> create_shape_from_collider(
          const collider_body& collider);

        /**
         * @brief Creates the body and shapes from queries of Rigidbody and
         * collider. Calling Jolt create body from the physics_system to allow
         * jolt to keep track of each shape and associate to each physics body.
         * It then moves that physics body into the jolt simulation. Allowing
         * Jolt to manipulate the numbers associated with it during
         * physics_step().
         *
         */
        void add_body(flecs::entity e,
                      const physics_body* body_opt,
                      const collider_body& collider,
                      const transform& location,
                      std::vector<JPH::BodyCreationSettings>& settings_list,
                      std::vector<flecs::entity>& entity_list);

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
        ref<contact_listener> m_contact_listener;

        /**
         * @brief Gives access to the physics system. Is given to jolt_api as
         * well. This is to allow us to give seperation of concerns. However,
         * may change now do to Jolt conflict and unhandled dangling pointers
         * within jolt_api.
         *
         */
        ref<JPH::PhysicsSystem> m_physics_system;
    };
};