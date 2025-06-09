#include <type_traits>
#include <Jolt/Jolt.h>
#include <physics/physics_3d/physics_context.hpp>
#include <physics/physics_3d/jolt/jolt_context.hpp>

namespace atlas::physics {

    void physics_context::create_bodies() {
        engine_create_physics_bodies();
    }

    void physics_context::clean_bodies() {
        engine_clean_physics_bodies();
    }

    void physics_context::run_physics_step() {
        engine_run_physics_step();
    }

    void physics_context::contact_added_event() {
        engine_run_contact_added();
    }

}