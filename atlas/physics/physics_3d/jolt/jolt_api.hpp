#pragma once
#include <physics/physics_3d/physics_api.hpp>
#include <physics/physics_3d/data/jolt_config.hpp>
namespace atlas::physics {
    class jolt_api : public physics_api {
        public:
            jolt_api(jolt_config p_config);

        private:
            jolt_config m_config;

    };
};