#include <drivers/jolt-cpp/jolt-imports.hpp>
#include <drivers/jolt-cpp/jolt_contact_listener.hpp>
#include <core/engine_logger.hpp>
#include <drivers/jolt-cpp/jolt_components.hpp>
#include <core/event/types.hpp>

namespace atlas::physics {

    contact_listener::contact_listener(event::event_bus& p_bus) : m_bus(&p_bus) {
        console_log_info("contact_listener initialized!!!");
    }

    JPH::ValidateResult contact_listener::OnContactValidate(
      const JPH::Body&,
      const JPH::Body&,
      JPH::RVec3Arg,
      const JPH::CollideShapeResult&) {
        return JPH::ValidateResult::AcceptContact;
    }

    void contact_listener::OnContactAdded(const JPH::Body& p_body1,
                                          const JPH::Body& p_body2,
                                          const JPH::ContactManifold&,
                                          JPH::ContactSettings&) {

        console_log_info("Collisions Added!");

        // Code that works well!
        // We can use GetUserData to fetch the flecs::entity::id() that we
        // assign when creating using BodyCreationSettings.mUserData parameter.
        // Then using the event system, we can then report back to the listeners
        // who subscribed to that, and send off these ID's when needed.
        event::collision_begin begin_event = {
            .entity1 = static_cast<uint64_t>(p_body1.GetUserData()),
            .entity2 = static_cast<uint64_t>(p_body2.GetUserData())
        };
        // uint64_t entity_id1 = static_cast<uint64_t>(p_body1.GetUserData());
        // uint64_t entity_id2 = static_cast<uint64_t>(p_body2.GetUserData());

        console_log_info("Collided Added with Entity ID = {}", begin_event.entity1);
        console_log_info("Collided Added with Entity ID = {}", begin_event.entity2);

        // For Event system to handle when collision begins
        m_bus->publish(begin_event);
    }

    void contact_listener::OnContactPersisted(const JPH::Body& p_body1,
                                              const JPH::Body& p_body2,
                                              const JPH::ContactManifold&,
                                              JPH::ContactSettings&) {
        // console_log_info("Collisions Persisted!");
        // For Event system to handle when collision continues always
        event::collision_persisted persisted_event = {
            .entity1 = static_cast<uint64_t>(p_body1.GetUserData()),
            .entity2 = static_cast<uint64_t>(p_body2.GetUserData())
        };

        m_bus->publish(persisted_event);
    }

    void contact_listener::OnContactRemoved(const JPH::SubShapeIDPair&) {
        console_log_info("Collisions Removed!");
        // For Event system to handle when collision ends
    }

}