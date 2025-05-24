#pragma once
#include <core/core.hpp>
#include <physics/jolt-cpp/jolt_components.hpp>
#include <physics/physics_3d/jolt/jolt_context.hpp>

namespace atlas::physics {
    
    class physics_engine {
        public:
            physics_engine() = default;
            physics_engine(jolt::jolt_settings p_settings, ref<physics_context>& p_engine);

            void start_runtime();
            void physics_step();
            void stop_runtime();
            void run_contact_add();

        private:
            jolt::jolt_settings m_settings;
            ref<physics_context> m_engine_api;
    };
};