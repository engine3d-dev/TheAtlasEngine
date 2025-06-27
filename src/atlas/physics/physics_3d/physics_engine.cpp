#include <physics/physics_3d/physics_engine.hpp>

namespace atlas::physics {

    physics_engine::physics_engine(const jolt_settings& p_settings,
                                   const ref<physics_context>& p_engine,
                                   const ref<physics_api>& p_user_api)
      : m_settings(p_settings)
      , m_engine_api(p_engine)
      , m_backend_api(p_user_api) {};

    void physics_engine::start_runtime() {
        m_engine_api->create_bodies();
    }

    void physics_engine::physics_step() {
        m_backend_api->update_jolt_values();
        m_engine_api->run_physics_step();
        m_backend_api->update_atlas_values();
    }

    void physics_engine::stop_runtime() {
        m_engine_api->clean_bodies();
    }

    void physics_engine::run_contact_add() {
        m_engine_api->contact_added_event();
    }
}