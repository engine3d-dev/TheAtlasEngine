#include <core/engine_logger.hpp>
#include <physics/physics_3d/physics_api.hpp>

namespace atlas::physics {
    void physics_api::update_jolt_values() {
        retrieve_values();
    }

    void physics_api::update_atlas_values() {
        return_values();
    }
}