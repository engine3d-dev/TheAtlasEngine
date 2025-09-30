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
        // Pass through function to allow private virtual functions to be called
        // publically without messing up the virtual table.
        void create_bodies();
        void clean_bodies();
        void run_physics_step();
        void contact_added_event();
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
        virtual void engine_create_physics_bodies() = 0;
        virtual void engine_clean_physics_bodies() = 0;
        virtual void engine_run_physics_step() = 0;
        virtual void engine_run_contact_added() = 0;
        virtual void update_simulation(float p_delta_time) = 0;

        virtual ref<JPH::PhysicsSystem>& set_physics_instance() = 0;
    };

    ref<physics_context> initialize_physics_context(const jolt_settings& p_settings);
};