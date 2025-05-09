#include <engine_logger.hpp>
#include <core/scene/scene.hpp>
#include <Jolt/Jolt.h>
#include <Jolt/Physics/Body/Body.h>
#include <physics/jolt-cpp/jolt_contact_listener.hpp>
#include <physics/physics_3d/data/contact.hpp>

#include <flecs.h>

namespace atlas::physics {

    contact_listener::contact_listener()
    {
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
        m_registry = *scene;
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
                                          JPH::ContactSettings&) {

        auto entity1 = m_registry.entity(body1.GetID());

        auto entity2 = m_registry.entity(body2.GetID());

        contact_event event;

        event.entity_a = entity1.id();
        event.entity_b = entity2.id();
        event.manifold = manifold;

        m_contact_list.push_back(
          event);
    }

    void contact_listener::OnContactPersisted(const JPH::Body&,
                                              const JPH::Body&,
                                              const JPH::ContactManifold&,
                                              JPH::ContactSettings&) {

        //!  @note We dont need this for now so I am going to ignore it
    }

    void contact_listener::OnContactRemoved(const JPH::SubShapeIDPair&) {
        //! @note We dont need this for now so I am going to ignore it
        return;
    }

    void contact_listener::clear_events()
    {
        m_contact_list.clear();
    }

}