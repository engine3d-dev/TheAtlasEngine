#include <physics/jolt-cpp/jolt_components.hpp>
#include <physics/jolt-cpp/jolt_collision_manager.hpp>
#include <physics/jolt-cpp/jolt_collision.hpp>

namespace atlas::physics {

    jolt_collision::jolt_collision() {
        m_id = 0;
    }

    jolt_collision::jolt_collision(flecs::entity_t p_entity_id) {
        m_id = p_entity_id;

        ref<world_scope> world_object =
          system_registry::get_world("Editor World");
        if (!world_object) {
            console_log_error("World not found.");
            return;
        }

        ref<scene_scope> scene = world_object->get_scene("LevelScene");
        if (!scene) {
            console_log_error("Scene not found.");
            return;
        }

        flecs::world m_registry = *scene;
        flecs::entity target(m_registry, p_entity_id);

        target.set<collider_event>({ .id = p_entity_id });

        jolt_collision_manager::subscribe_action(m_id, *this);
    }

    void jolt_collision::collision_added(jolt::contact_event& p_event) {
        on_collision_added(p_event);
    }

    void jolt_collision::on_collision_added(jolt::contact_event& p_event) {
        console_log_info(
          "Default collision added:\nEvent:\n\tMain Entity ID: {}\n\tCollider "
          "Entity ID: {}\n\tManifold Offset: ({},{},{})\n\tIs A Sensor "
          "Trigger: {}",
          p_event.entity_a,
          p_event.entity_b,
          p_event.manifold.mBaseOffset.GetX(),
          p_event.manifold.mBaseOffset.GetY(),
          p_event.manifold.mBaseOffset.GetZ(),
          p_event.settings.mIsSensor);
    }
}