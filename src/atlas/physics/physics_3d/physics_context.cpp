#include <type_traits>
#include <Jolt/Jolt.h>
#include <physics/physics_3d/physics_context.hpp>
#include <drivers/jolt-cpp/jolt_context.hpp>

namespace atlas::physics {

    ref<physics_context> initialize_physics_context(const jolt_settings& p_settings) {
        return create_ref<jolt_context>(p_settings);
    }

}