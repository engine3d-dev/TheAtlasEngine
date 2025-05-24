#include <physics/physics_3d/physics_engine.hpp>

namespace atlas::physics {

    physics_engine::physics_engine(jolt::jolt_settings p_settings,
                                   ref<physics_context>& p_engine)
      : m_settings(p_settings)
      , m_engine_api(p_engine) {};

    void physics_engine::start_runtime() {
        m_engine_api->create_bodies();
    }

    void physics_engine::physics_step() {
        m_engine_api->run_physics_step();
    }

    void physics_engine::stop_runtime() {
        m_engine_api->clean_bodies();
    }

    void physics_engine::run_contact_add() {
        m_engine_api->contact_added_event();
    }
}