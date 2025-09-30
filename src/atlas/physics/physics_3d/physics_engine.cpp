#include <physics/physics_3d/physics_engine.hpp>
#include <drivers/jolt-cpp/jolt_api.hpp>

namespace atlas::physics {

    physics_engine::physics_engine(const jolt_settings& p_settings, const jolt_config& p_config, flecs::world* p_registry) {
        // ref<jolt_context> engine_access = create_ref<jolt_context>(p_settings);
        // m_engine_api = create_ref<jolt_context>(p_settings);
        // m_engine_api = engine_access;
        m_physics_context = initialize_physics_context(p_settings);

        jolt_api user_api(p_config, m_physics_context->physics_instance(), *p_registry);

        m_backend_api = create_ref<jolt_api>(user_api);
    };

    void physics_engine::start() {
        m_physics_context->create_bodies();
    }

    void physics_engine::update() {
        m_backend_api->update_jolt_values();
        m_physics_context->run_physics_step();
        m_backend_api->update_atlas_values();

        // Execute collisions that happen with the collision manager
        m_physics_context->contact_added_event();
    }

    void physics_engine::stop() {
        m_physics_context->clean_bodies();
    }

}