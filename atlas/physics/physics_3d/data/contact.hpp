#pragma once

#include <Jolt/Jolt.h>
#include <Jolt/Physics/Collision/ContactListener.h>
#include <flecs.h>

// Tells whether or not entities have contact listening abilities (user defined).
struct contact {
    // Will have more later
    bool contact_added = false;
    bool contact_persisted = false;
    bool contact_removed = false;
};