#include <physics/physics_engine.hpp>

namespace atlas {
    physics_engine::physics_engine(flecs::world* p_registry, const physics_settings&) : m_registry(p_registry) {
    }

    void physics_engine::start() {
        
    }

    void physics_engine::stop() {}


    void current_scene(flecs::world* p_registry);
};