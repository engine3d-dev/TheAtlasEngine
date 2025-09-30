#pragma once
#include <flecs.h>
#include <core/core.hpp>
#include <glm/glm.hpp>
#include <core/scene/components.hpp>
#include <physics/components.hpp>

namespace atlas {
    /**
     * @brief Enum for toggling between physics context implementations
     * 
     * TODO: Probably consider a way that makes this not tied to the enum for toggling between varied implementations.
    */
    enum class physics_api : uint8_t {
        jolt
    };

    enum thread_type : uint8_t {
        default_system = 0,
        job_system = 1,
    };


    // struct physics_settings {};

    //! @brief Because Jolt has parameters it uses, these might be moved entirely into jolt_context
    // Rather then inside of its own struct, potentially. For simplicity
    struct physics_settings {
        // Global gravity vector for all in scene
        glm::vec3 gravity = glm::vec3(0.0f, -9.80665f, 0.0f);

        uint32_t allocation_count = 10 * 1024 * 1024;
        physics_api api_context = physics_api::jolt;
        thread_type multithread_context = thread_type::default_system;
        uint32_t thread_count = std::max(1u, std::thread::hardware_concurrency()-2);
        uint32_t max_jobs_power = 10;
        uint32_t max_barriers = thread_count * 16;
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

        // Time step for fps within the physics
        // If false set to 1/maxFps
        float fixed_time_step = 1.0f / 60.0f;
        bool use_fixed_timestep = false;

        // solver for velocity and position
        uint32_t position_iterations = 1;
        uint32_t velocity_iterations = 8;

        // When to turn objects on and off depending on speed
        float sleep_velocity_threshold = 0.05f;
        float sleep_angular_velocity_threshold = 0.05f;
    };

    /**
     * @brief Abstraction to toggle between implementation context
     * 
     * Pure virtual base class to act to have different implementations
    */
    class physics_context {
    public:
        virtual ~physics_context() = default;

        //! @brief Start simulation
        void start() { return start_simulation(); }

        //! @brief Stop Simulation and Post cleanup of the simulation
        void stop() { return stop_simulation(); }

        void update(float p_delta_time) { return update_simulation(p_delta_time); }

        void add_body(uint32_t p_id, const physics_body* p_body, const transform* p_transform) { return create_body(p_id, p_body, p_transform); }

        // void add_body_with_collider(const physics_body* p_body, const collider_body* p_collider) { return create_body_with_collider(p_body, p_collider); }

        void add_box_collider(flecs::entity p_entity, const physics_body* p_body, const box_collider* p_collider, const transform* p_transform) {
            return emplace_box_collider(p_entity, p_body, p_collider, p_transform);
        }

        void add_box_collider(uint32_t p_id, const box_collider* p_collider, const transform* p_transform) { return emplace_box_collider(p_id, p_collider, p_transform); }

        void add_sphere_collider(uint32_t p_id, const sphere_collider* p_collider, const transform* p_transform) { return emplace_sphere_collider(p_id, p_collider, p_transform); }

        void add_capsule_collider(uint32_t p_id, const capsule_collider* p_collider, const transform* p_transform) { return emplace_capsule_collider(p_id, p_collider, p_transform); }

        void position_and_rotation(uint32_t p_id, const glm::vec3& p_position, const glm::vec4& p_rotation) { return set_position_and_rotation(p_id, p_position, p_rotation); }

        void set_test_add_body(flecs::entity p_entity, const physics_body* p_body, const box_collider* p_collider, const transform* p_transform) { return emplace_set_test_add_body(p_entity, p_body, p_collider, p_transform); }

        void set_experiemental_add_body(flecs::entity p_entity) { return experiemental_add_body(p_entity); }

        void prepare() { return prepare_physics(); }

    protected:
        virtual void start_simulation() = 0;

        virtual void stop_simulation() = 0;

        virtual void prepare_physics() = 0;

        virtual void update_simulation(float p_delta_time) = 0;

        virtual void experiemental_add_body(flecs::entity p_entity) = 0;

        //! @brief This is to modify the position and rotation to the specific body ID.
        virtual void set_position_and_rotation(uint32_t p_body_id, const glm::vec3& p_position, const glm::vec4& p_rotation) = 0;

        // void create_physics_body(const physics_body* p_body)
        virtual void create_body(uint32_t p_id, const physics_body* p_body, const transform* p_transform) = 0;

        // Every collider require a physics body
        // void create_box_collider(const physics_body* p_body, const box_collider* p_collider)
        // virtual void create_body_with_collider(const physics_body* p_body, const collider_body* p_collider) = 0;

        virtual void emplace_box_collider(flecs::entity p_entity, const physics_body* p_body, const box_collider* p_collider, const transform* p_transform) = 0;

        virtual void emplace_box_collider(uint32_t p_id, const box_collider* p_collider, const transform* p_transform) = 0;

        virtual void emplace_sphere_collider(uint32_t p_id, const sphere_collider* p_collider, const transform* p_transform) = 0;

        virtual void emplace_capsule_collider(uint32_t p_id, const capsule_collider* p_collider, const transform* p_transform) = 0;

        virtual void emplace_set_test_add_body(flecs::entity p_entity, const physics_body* p_body, const box_collider* p_collider, const transform* p_transform) = 0;

        virtual void post_cleanup() = 0;
    };

    //! @brief Initializes the physics-specific backend context
    ref<physics_context> initialize_physics_context(const physics_settings& p_settings);

};