#pragma once
#include <core/core.hpp>
#include <drivers/jolt-cpp/jolt_components.hpp>
#include <physics/physics_context.hpp>
#include <core/event/event_bus.hpp>

namespace atlas::physics {

    /**
     * @brief The manager class for all physics engines. Manages the physics
     * contexts and the collision engines.
     *
     */
    class physics_engine {
    public:
        // Required by maps but should not be used in anyother circumstance.
        physics_engine() = default;
        physics_engine(const jolt_settings& p_settings,
                       flecs::world& p_registry,
                       event::event_bus& p_bus);

        /**
         * @brief The runtime is specifically the way to start the physics,
         * utilizing defined data from level_scene and eventually any scene to
         * create a physics scene and manage physics step.
         *
         */
        void start();

        /**
         * @brief Runs a single frame of the simulation. Should at minimum be
         * called 60 fps. If below, it must be called twice.
         *
         */
        void update(float p_delta_time);

        /**
         * @brief Deletes all physics bodies and shapes. Preps itself for
         * runtime to be called again.
         *
         */
        void stop();

    private:
        flecs::world* m_registry;
        ref<physics_context> m_physics_context;
        flecs::query<physics_body> m_physics_bodies;
        flecs::query<transform, physics_body, box_collider>
          m_query_box_collider;
        flecs::query<transform, physics_body, sphere_collider>
          m_query_sphere_collider;
        flecs::query<transform, physics_body, capsule_collider>
          m_query_capsule_collider;
        event::event_bus* m_bus;
    };
};