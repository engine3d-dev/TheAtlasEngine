#include <physics/physics_3d/physics_engine.hpp>
#include <drivers/jolt-cpp/jolt_api.hpp>

namespace atlas::physics {

    physics_engine::physics_engine(const jolt_settings& p_settings, const jolt_config& p_config, flecs::world* p_registry) : m_settings(p_settings) {
        ref<jolt_context> engine_access = create_ref<jolt_context>(p_settings);
        m_engine_api = create_ref<jolt_context>(p_settings);
        m_engine_api = engine_access;

        jolt_api user_api(p_config, engine_access->m_physics_system, *p_registry);

        m_backend_api = create_ref<jolt_api>(user_api);
    };

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