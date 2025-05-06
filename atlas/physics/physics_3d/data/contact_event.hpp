#pragma once

#include <Jolt/Jolt.h>
#include <Jolt/Physics/Collision/ContactListener.h>
#include <flecs.h>

struct contact_event {
    uint64_t entity_a = 0;
    uint64_t entity_b = 0;
    JPH::ContactManifold manifold;
};