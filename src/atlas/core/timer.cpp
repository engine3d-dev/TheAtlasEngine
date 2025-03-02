#include <core/timer.hpp>
#include <core/update_handlers/sync_update.hpp>

namespace atlas::time {
    static uint32_t g_CollisionSteps = 0;
    void update_physics_step() {
        g_CollisionSteps = 1 + (60 * (sync_update::delta_time()));
    }

    float physcs_step() {
        return g_CollisionSteps;
    }
};