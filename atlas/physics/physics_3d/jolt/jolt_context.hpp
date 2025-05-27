#pragma once

#include <physics/jolt-cpp/jolt_contact_listener.hpp>
#include <core/core.hpp>

#include <physics/physics_3d/physics_context.hpp>
#include <physics/jolt-cpp/jolt_components.hpp>
#include <components/transform.hpp>
#include <physics/physics_3d/jolt/interface/jolt_broad_phase.hpp>

namespace atlas::physics {

    class jolt_context : public physics_context {
    public:
        jolt_context(jolt::jolt_settings p_settings);
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
        void engine_create_physics_bodies() override;

        /**
         * @brief Removes all physics bodies and shapes from level_scene.
         *
         * @warning Similar to create this only works with level_scene as of
         * right now.
         *
         */
        void engine_clean_physics_bodies() override;

        /**
         * @brief Runs the actual physics. Can be called multiple times in one
         * frame. Runs the all calculation in physics. Threads must be defined
         * in the jolt_config in order for this to work correctly.
         *
         */
        void engine_run_physics_step() override;

        /**
         * @brief Runs a given <contact added> the frame the contact was
         * created. Allows for all contacts to run after the physics has
         * finished as to stop thread collision issues.
         *
         */
        void engine_run_contact_added() override;

        /**
         * @brief Jolt enforces the system to handle the shapes created instead
         * of itself. Otherwise the ref will reach 0 and the object will crash
         * as the shape is removed from the bopy. So a map was created to
         * maintain the shapes scope.
         *
         * @param id shape id to be stored in map.
         * @param shape the shape that was created.
         */
        void store_shape(uint64_t id, const JPH::RefConst<JPH::Shape>& shape);

        /**
         * @brief Creates a shape given a jolt collider
         *
         * @param e the entity who shape you are creating
         * @param collider the entities jolt collider
         * @return JPH::RefConst<JPH::Shape> The shape needs to be created and
         * stored for ref counting and scope.
         */
        JPH::RefConst<JPH::Shape> create_shape_from_collider(
          flecs::entity e,
          const collider_body& collider);

        void add_body(flecs::entity e,
              const physics_body& body,
              const collider_body& collider,
              const transform_physics& location,
              std::vector<JPH::BodyCreationSettings>& settings_list,
              std::vector<flecs::entity>& entity_list);

        void add_body_collider(flecs::entity e,
                       const collider_body& collider,
                       const transform_physics& location,
                       std::vector<JPH::BodyCreationSettings>& settings_list,
                       std::vector<flecs::entity>& entity_list);

        //! @note Must be defined before physics can be initialized otherwise
        //! jolt cannot be created properly.
        jolt::jolt_settings m_settings;

        ref<JPH::TempAllocatorImpl> m_temp_allocator;
        ref<JPH::PhysicsSystem> m_physics_system;
        scope<JPH::JobSystemThreadPool> m_thread_system;

        ref<broad_phase_layer_interface> m_broad_phase_layer_interface;
        ref<object_vs_broadphase_layer> m_object_vs_broadphase_filter;
        ref<object_layer_pair_filter> m_object_layer_pair_filter;

        ref<contact_listener> m_contact_listener;

        //! @note This map is to create the scope for shapes created by Jolt.
        std::unordered_map<uint64_t, JPH::RefConst<JPH::Shape>>
          m_shape_registry;
    };
};