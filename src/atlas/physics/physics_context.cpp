#include <type_traits>
#include <Jolt/Jolt.h>
#include <physics/physics_context.hpp>
#include <drivers/jolt-cpp/jolt_context.hpp>
#include <core/event/event_bus.hpp>

namespace atlas::physics {

    ref<physics_context> initialize_physics_context(
      const jolt_settings& p_settings,
      event::event_bus& p_bus) {
        return create_ref<jolt_context>(p_settings, p_bus);
    }

}