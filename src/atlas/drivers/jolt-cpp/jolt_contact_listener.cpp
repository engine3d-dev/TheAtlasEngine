#include <core/engine_logger.hpp>
#include <Jolt/Jolt.h>
#include <Jolt/Physics/Body/Body.h>
#include <drivers/jolt-cpp/jolt_contact_listener.hpp>
#include <drivers/jolt-cpp/jolt_components.hpp>

namespace atlas::physics {

    contact_listener::contact_listener() {
        console_log_info("contact_listener initialized!!!");
    }

    JPH::ValidateResult contact_listener::OnContactValidate(
      const JPH::Body&,
      const JPH::Body&,
      JPH::RVec3Arg,
      const JPH::CollideShapeResult&) {
        return JPH::ValidateResult::AcceptContact;
    }

    void contact_listener::OnContactAdded(const JPH::Body&,
                                          const JPH::Body&,
                                          const JPH::ContactManifold&,
                                          JPH::ContactSettings&) {

        console_log_info("Getting to Collisions!\n");

        // For Event system to handle when collision begins
    }

    void contact_listener::OnContactPersisted(const JPH::Body&,
                                              const JPH::Body&,
                                              const JPH::ContactManifold&,
                                              JPH::ContactSettings&) {
        // For Event system to handle when collision continues always
    }

    void contact_listener::OnContactRemoved(const JPH::SubShapeIDPair&) {
        // For Event system to handle when collision ends
    }

}