#pragma once
#include <core/core.hpp>
#include <Jolt/Jolt.h>
#include <drivers/jolt-cpp/jolt_components.hpp>
#include <drivers/jolt-cpp/jolt-imports.hpp>

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

        ref<JPH::PhysicsSystem>& physics_instance() { return set_physics_instance(); }

        /**
         * @note:
         *  Many more in the future like:
         *  batching physics bodies
         *  chucking engine location
         *  shifting coordinates
         */

    private:
        virtual void create_bodies() = 0;
        virtual void destroy_bodies() = 0;
        virtual void execute_collisions() = 0;
        virtual void update_simulation(float p_delta_time) = 0;

        virtual ref<JPH::PhysicsSystem>& set_physics_instance() = 0;
    };

    ref<physics_context> initialize_physics_context(const jolt_settings& p_settings);
};