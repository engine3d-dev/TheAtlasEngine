#pragma once
#include <core/core.hpp>
#include <core/scene/scene.hpp>
#include <physics/jolt-cpp/jolt-imports.hpp>

namespace atlas::physics {
    class physics_scene {
    public:
        physics_scene() = default;
        physics_scene(const scene_scope* p_scene_ctx);

        bool is_enabled() const { return m_is_physics_enabled; }

        //! @note OnRuntime Start/Stop for initiating the physics scene

        //! @note Used to instantiate the physics at runtime
        void on_runtime_start();

        //! @note Used to tell when the simulation stops at runtime
        void on_runtime_stop();

        //! @note Runs the update that uses the physics specific delta time
        //! @note Does the actual physics logic handler here
        void on_runtime_update(float p_physics_dt);

        void add_scene_object(const ref<scene_object>& p_current_object);

    private:
        // JPH::PhysicsSystem* m_PhysicsSystem = nullptr;
        bool m_is_physics_enabled =
          false; // used for ensuring that only when physics simulation is
                 // started that we tell it to stop
        scope<scene_scope> m_current_scene_ctx;

        ref<JPH::PhysicsSystem> m_physics_system;
    };
};