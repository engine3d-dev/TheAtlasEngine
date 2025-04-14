#include <cstdint>
#include <glm/glm.hpp>

namespace atlas::physics {
    /**
     * @brief Used to keep global data for player access and use. Tells how
     * physics bodies should act within a given scene by default.
     */
    struct jolt_config_data {
        // Global gravity vector for all in scene
        glm::vec3 gravity = glm::vec3(0.0f, -9.80665f, 0.0f);

        // Time step for fps within the physics
        // If false set to 1/maxFps
        float fixed_time_step = 1.0f / 60.0f;
        bool use_fixed_timestep = false;

        // solver for velocity and position
        uint32_t position_iterations = 6;
        uint32_t velocity_iterations = 6;

        // When to turn objects on and off depending on speed
        float sleep_velocity_threshold = 0.05f;
        float sleep_angular_velocity_threshold = 0.05f;

        //! @note In seconds
        float time_before_sleep = 5.0f;

        // What 1 unit refers to in meters
        float world_unit_scale = 1.0f;

        // Helps stop the lauching of objects during numerical/flaoting point
        // errors when collision happen bertween to objects.
        float contact_bias_factor = 0.2f;
        float restitution_threshold = 1.0f;

        bool enable_constraints = true;
        bool enable_contacts = true;
        bool enable_collision_callbacks = true;
    };
};