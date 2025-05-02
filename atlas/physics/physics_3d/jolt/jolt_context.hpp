#pragma once

#include <core/core.hpp>

#include <physics/physics_3d/physics_context.hpp>
#include <physics/physics_3d/data/jolt_settings.hpp>
#include <physics/physics_3d/jolt/interface/jolt_broad_phase.hpp>

namespace atlas::physics {
    
    class jolt_context : public physics_context {
        public:
            jolt_context(jolt_settings p_settings);
            ~jolt_context() override;
        private:
            void engine_create_physics_bodies() override;
            void engine_clean_physics_bodies() override;
            void engine_run_physics_step() override;

            void store_shape(uint64_t id, const JPH::RefConst<JPH::Shape>& shape);

            jolt_settings m_settings;

            ref<JPH::TempAllocatorImpl> m_temp_allocator;
            ref<JPH::PhysicsSystem> m_physics_system;
            scope<JPH::JobSystemThreadPool> m_thread_system;

            ref<broad_phase_layer_interface> m_broad_phase_layer_interface;
            ref<object_vs_broadphase_layer> m_object_vs_broadphase_filter;
            ref<object_layer_pair_filter> m_object_layer_pair_filter;

            std::unordered_map<uint64_t, JPH::RefConst<JPH::Shape>> m_shape_registry;
    };
};