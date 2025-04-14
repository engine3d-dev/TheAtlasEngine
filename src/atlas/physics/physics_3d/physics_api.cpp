#include <physics/physics_3d/jolt/jolt_physics_engine.hpp>
#include <core/scene/scene_object.hpp>

namespace atlas::physics {

    void create(ref<scene_object> physics_object) {

        if (physics_object == nullptr) {
            console_log_fatal("Missing physics scene object.");
            return;
        }

        if (physics_object->get<jolt_settings_data>() == nullptr) {
            console_log_fatal("Missing jolt_settings component.");
            return;
        }

        create_engine(*physics_object->get<jolt_settings_data>());
    }

}