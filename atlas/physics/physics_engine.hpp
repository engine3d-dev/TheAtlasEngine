#pragma once
#include <flecs.h>

namespace atlas {
    struct physics_settings {};

    class physics_engine {
    public:
        physics_engine() = default;
        physics_engine(flecs::world* p_registry, const physics_settings& p_settings={});

        void start();

        void stop();


        void current_scene(flecs::world* p_registry);

        // void add_velocity(const glm::vec3& p_velocity, )
    private:
        flecs::world* m_registry;
    };
};