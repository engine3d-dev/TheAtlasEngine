#pragma once
#include <core/core.hpp>
#include <drivers/jolt-cpp/jolt_components.hpp>
#include <drivers/jolt-cpp/jolt_api.hpp>
#include <physics/physics_3d/physics_context.hpp>
// #include <drivers/jolt-cpp/jolt_api.hpp>

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
        physics_engine(const jolt_settings& p_settings, const jolt_config& p_config, flecs::world& p_registry);

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

        /**
         * @brief Allows for contacts to be handled by the user right after
         * physics_step has fully run.
         *
         * @warning It is important that these stay distict until we have a job
         * manager to maintain them.
         *
         */
        // void execute_collisions();

    private:
        flecs::world* m_registry;
        ref<physics_context> m_physics_context;
        jolt_config m_jolt_config;

        /**
         * @brief Flecs querys deticated to reading and writing from jolt and
         * atlas. Targeting physics bodies.
         *
         */
        flecs::query<transform, collider_body> m_query_transform;
        flecs::query<physics_body> m_query_body;
    };
};