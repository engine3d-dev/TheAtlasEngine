#include <engine_logger.hpp>
#include <Jolt/Jolt.h>
#include <Jolt/Physics/Body/Body.h>
#include <physics/jolt-cpp/jolt_contact_listener.hpp>
#include <physics/jolt-cpp/jolt_components.hpp>
#include <physics/jolt-cpp/jolt_collision_manager.hpp>

#include <flecs.h>

namespace atlas::physics {

    ref<jolt_collision_manager> m_manager;

    contact_listener::contact_listener() {
        ref<world_scope> world_object =
          system_registry::get_world("Editor World");
        if (!world_object) {
            console_log_error("World not found.");
            return;
        }

        m_scene = world_object->get_scene("LevelScene");
        if (!m_scene) {
            console_log_error("Scene not found.");
            return;
        }
        m_registry = *m_scene;

        m_manager = jolt_collision_manager::initialize("level_scene_collision");
    }

    JPH::ValidateResult contact_listener::OnContactValidate(
      const JPH::Body&,
      const JPH::Body&,
      JPH::RVec3Arg,
      const JPH::CollideShapeResult&) {
        return JPH::ValidateResult::AcceptContact;
    }

    void contact_listener::OnContactAdded(const JPH::Body& body1,
                                          const JPH::Body& body2,
                                          const JPH::ContactManifold& manifold,
                                          JPH::ContactSettings& settings) {

        console_log_info("Getting to Collisions!\n");

        flecs::entity_t entity_id1 =
          static_cast<flecs::entity_t>(body1.GetUserData());
        flecs::entity_t entity_id2 =
          static_cast<flecs::entity_t>(body2.GetUserData());

        flecs::entity entity1 = m_registry.entity(entity_id1);
        flecs::entity entity2 = m_registry.entity(entity_id2);

        if (!entity1.is_alive()) {
            console_log_fatal("First entity not found! ID: {}", entity_id1);
            return;
        }

        if (!entity2.is_alive()) {
            console_log_fatal("Second entity not found! ID: {}", entity_id2);
            return;
        }

        contact_event event;

        event.entity_a = entity_id1;
        event.entity_b = entity_id2;
        event.manifold = manifold;
        event.settings = settings;

        m_contacts_added.push_back(event);
    }

    void contact_listener::OnContactPersisted(const JPH::Body&,
                                              const JPH::Body&,
                                              const JPH::ContactManifold&,
                                              JPH::ContactSettings&) {

        // We dont need this for now so I am going to ignore it
        return;
    }

    void contact_listener::OnContactRemoved(const JPH::SubShapeIDPair&) {
        // We dont need this for now so I am going to ignore it

        // flecs::entity_t entity1 = m_registry.entity(pair.GetBody1ID().);
        // if(!entity1) {

        //     return;
        // }
        return;
    }

    void contact_listener::clear_events() {
        m_contacts_added.clear();
        m_contacts_persisted.clear();
        m_contacts_removed.clear();
    }

    void contact_listener::run_events_removed() {
        // We dont need this yet either;
    }

    void contact_listener::run_events_persisted() {
        // We dont need this for now so I am going to ignore it
        return;
    }

    void contact_listener::run_events_added() {
        contact_event event;
        // console_log_fatal("Contact Size: {}\n", m_contacts_added.size());
        for (uint64_t i = 0; i < m_contacts_added.size(); i++) {
            event = m_contacts_added.back();

            flecs::entity target_entity(m_registry, event.entity_a);

            if (target_entity.is_alive() &&
                target_entity.has<collider_event>()) {
                const collider_event* trigger_ptr =
                  target_entity.get<collider_event>();

                m_manager->run_collision_added(trigger_ptr->id, event);
            }
            else {
                console_log_error("Object does not exsist: {}, or does not "
                                  "have a collider event: {}\n",
                                  target_entity.is_alive(),
                                  target_entity.has<collider_event>());
            }
            m_contacts_added.pop_back();
        }
    }
}