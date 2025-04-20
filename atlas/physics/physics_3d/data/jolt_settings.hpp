#pragma once
#include <cstdint>
#include <glm/glm.hpp>
#include <thread>
namespace atlas::physics {

    /**
     * @brief A data structure to give to flecs and get the physics
     * engine prepped for Init() phase.
     * ---------------------------------------------------
     * @note This includes global configs for each scene and how the
     * physics engine will behave in a paticular scene.
     * --------------------------------------------------
     * @remark world_bounds_min && world_bounds_max: These values are
     * arbitrary until further investigation happens and are more of a
     * reminder that floating point error exsists. True max values
     * should be determined in the near future.
     *
     */
    struct physics_settings {

        // For job system
        uint32_t physics_threads = std::thread::hardware_concurrency();
        bool is_multithreaded = true;

        // Collision detection
        float broadphase_scale = 1.0f;
        bool use_4_layer_broadphase = true;
        uint32_t default_collision_group = 0;

        // Filtering out specifica collisions
        uint32_t collision_filter_layer_count = 32;
        uint32_t default_filter_mask = 0xFFFFFFFF;

        // Scene position in the world
        glm::vec3 physics_scene_position = glm::vec3(0.0f, 0.0f, 0.0f);

        glm::vec3 world_bounds_min = glm::vec3(-500.0f, -500.0f, -500.0f);

        glm::vec3 world_bounds_max = glm::vec3(500.0f, 500.0f, 500.0f);
        // Max memory size per scene
        uint32_t max_bodies = 16384;
        uint32_t max_body_pairs = 32768;
        uint32_t max_contact_constraints = 8192;

        //! @note FIXME: maybe add a debug bool here
    };
};
