#pragma once
#include <core/core.hpp>
#include <physics/jolt-cpp/jolt_components.hpp>
#include <physics/physics_3d/jolt/jolt_context.hpp>
#include <physics/physics_3d/physics_api.hpp>

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
                       const ref<physics_context>& p_engine,
                       const ref<physics_api>& p_user_api);

        /**
         * @brief The runtime is specifically the way to start the physics,
         * utilizing defined data from level_scene and eventually any scene to
         * create a physics scene and manage physics step.
         *
         */
        void start_runtime();

        /**
         * @brief Runs a single frame of the simulation. Should at minimum be
         * called 60 fps. If below, it must be called twice.
         *
         */
        void physics_step();

        /**
         * @brief Deletes all physics bodies and shapes. Preps itself for
         * runtime to be called again.
         *
         */
        void stop_runtime();

        /**
         * @brief Allows for contacts to be handled by the user right after
         * physics_step has fully run.
         *
         * @warning It is important that these stay distict until we have a job
         * manager to maintain them.
         *
         */
        void run_contact_add();

    private:
        jolt_settings m_settings;
        ref<physics_context> m_engine_api;
        ref<physics_api> m_backend_api;
    };
};