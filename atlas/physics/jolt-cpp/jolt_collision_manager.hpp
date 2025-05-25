#pragma once

#include <cstdint>
#include <physics/jolt-cpp/jolt_collision.hpp>
namespace atlas::physics {

    class jolt_collision_manager {
    public:
        jolt_collision_manager() = default;

        /**
         * @brief Creates all the neccessary starting components for collisions
         * to begin working. Is utilized by the physics engine to run user
         * defined collisions in a simple way.
         *
         * @param p_tag Creates a tag for the collision manager so debugging
         * later will be simplied. Specifically when there are more then one
         * scenes in the game world at once.
         *
         * @return scope<jolt_collision_manager> is the way the engine can run
         * the non-statics collision functions.
         */
        static scope<jolt_collision_manager> initialize(
          const std::string& p_tag);

        /**
         * @brief Connects an entity to a defualt or user defined
         * jolt_collision. Runs the functions of jolt_collision passing the
         * arguments for the user to utilize.
         *
         * @param p_id Generally a way to connect the id of a flecs::entity_t to
         * a specified jolt_collision
         * @param p_event Describes the collision that took place
         */
        void run_collision_added(uint64_t p_id, jolt::contact_event& p_event);
        // run_collision_persisted(uint64_t id);
        // run_collision_persisted(uint64_t id);

        /**
         * @brief Allows the user to subscribe their own collision_handlers to
         * the map and connecting those  handlers to entities that they define.
         *
         * @param p_id Used to connect an entity to a collision instance.
         * @param p_collision_action The collision handler to create actions
         * based on the collision given.
         */
        static void subscribe_action(uint64_t p_id,
                                     jolt_collision& p_collision_action);

    private:
        static std::unordered_map<uint64_t, jolt_collision> m_handlers;
        static std::string m_tag;
    };
};