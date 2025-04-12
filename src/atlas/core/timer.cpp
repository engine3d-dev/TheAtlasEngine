#include <core/timer.hpp>
#include <core/application.hpp>

namespace atlas::time {
    static uint32_t g_collision_steps = 0;
    void update_physics_step() {
        g_collision_steps = 1 + (60 * (application::delta_time()));
    }

    float physcs_step() {
        return g_collision_steps;
    }
};