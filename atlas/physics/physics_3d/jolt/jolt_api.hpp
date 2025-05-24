#pragma once
#include <physics/physics_3d/physics_api.hpp>
#include <physics/jolt-cpp/jolt_components.hpp>
namespace atlas::physics {
    class jolt_api : public physics_api {
        public:
            jolt_api(jolt::jolt_config p_config);

        private:
            jolt::jolt_config m_config;

    };
};