#pragma once
#include <core/core.hpp>
#include <Jolt/Jolt.h>
#include <drivers/jolt-cpp/jolt_components.hpp>
#include <drivers/jolt-cpp/jolt-imports.hpp>
#include <physics/components.hpp>
#include <core/scene/components.hpp>
#include <flecs.h>

namespace atlas::physics {
    /**
     * @brief The context is the way to interact with only the engine. It is the
     * api for all the background funcitons and information that the user
     * shouldn not see. It is a virtual based type erasure class so thatmany
     * engines can implement the same functions. Specifically realted to backend
     * engine creation like batching, starting runtime, etc...
     *
     */
    class physics_context {
    public:
        virtual ~physics_context() = default;
        void add_bodies() { return create_bodies(); }
        void destroy() { return destroy_bodies(); }
        void update_collision_events() { return execute_collisions(); }
        void update(float p_delta_time) { return update_simulation(p_delta_time); }

        void prepare() { return prepare_and_finalize(); }

        ref<JPH::PhysicsSystem>& physics_instance() { return set_physics_instance(); }

        void add_box_collider(flecs::entity p_entity, const physics_body* p_body, const box_collider* p_collider) { return emplace_box_collider(p_entity, p_body, p_collider); }

        // void position_and_rotation(flecs::entity p_entity, const physics_body* p_body, const box_collider* p_collider, const transform* p_transform) { return set_position_rotation(p_entity, p_body, p_collider, p_transform); }

        transform read_transform(uint32_t p_id) { return context_read_transform(p_id); }

        physics_body read_physics_body(uint32_t p_id) { return context_read_physics_body(p_id); }

    private:
        virtual void create_bodies() = 0;
        
        virtual void destroy_bodies() = 0;

        virtual void execute_collisions() = 0;

        // new API's for creating physics-specific bodies
        virtual void emplace_box_collider(flecs::entity p_entity, const physics_body* p_body, const box_collider* p_collider) = 0;

        // virtual void set_position_rotation(flecs::entity p_entity, const physics_body* p_body, const box_collider* p_collider, const transform* p_transform) = 0;

        virtual transform context_read_transform(uint32_t p_id) = 0;

        virtual physics_body context_read_physics_body(uint32_t p_id) = 0;

        virtual void prepare_and_finalize() = 0;
        virtual void update_simulation(float p_delta_time) = 0;

        virtual ref<JPH::PhysicsSystem>& set_physics_instance() = 0;
    };

    ref<physics_context> initialize_physics_context(const jolt_settings& p_settings);
};