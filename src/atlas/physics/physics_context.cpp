#include <physics/physics_context.hpp>
#include <drivers/jolt-cpp-experiemental/jolt_context.hpp>

namespace atlas {
    ref<physics_context> initialize_physics_context(const physics_settings& p_settings) {
        switch (p_settings.api_context) {
        case physics_api::jolt:
            return create_ref<physics::experiemental::jolt_context>(p_settings);
        }

        assert(false);
    }
};