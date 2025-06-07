#pragma once
#include <core/core.hpp>
#include <flecs.h>
#include <physics/physics_3d/physics_api.hpp>
#include <scene/components.hpp>
#include <physics/jolt-cpp/jolt_components.hpp>
namespace atlas::physics {
    class jolt_api : public physics_api {
    public:
        jolt_api(jolt_config& p_config,
                 ref<JPH::PhysicsSystem>& p_physics_system, flecs::world& p_registery);

    private:
        jolt_config& m_config;
        ref<JPH::PhysicsSystem>& m_physics_system;
        flecs::world m_registery;

        flecs::query< transform, collider_body> m_read_transform;
        flecs::query<physics_body> m_read_body;

        flecs::query< transform, collider_body> m_query_transform;
        flecs::query<physics_body> m_query_body;

        void retrieve_values() override;
        void return_values() override;
    };
};