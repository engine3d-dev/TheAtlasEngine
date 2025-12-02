#pragma once
#include <flecs.h>
#include <variant>
#include <tuple>

namespace atlas {

    /**
     * @brief Creates a game object that extends the flecs::entity
     *
     * Provides our own construct of API's that handles any workload around some
     * of the raw flecs API's that can still be used by other flecs API's
     * without completely interacting touching raw flecs API
     */
    class scene_object : public flecs::entity {
    public:
        scene_object() = default;

        scene_object(flecs::world_t* p_registry, flecs::entity_t p_id);

        scene_object(const flecs::entity& p_base);

        explicit scene_object(flecs::entity& p_base);

        /**
         * @brief sets the entity to be a parent of the specified entity
         *
         * @param p_entity is the specified entity to specify as the parent.
         *
         *
         * Example Usage:
         *
         * ```C++
         *
         * atlas::scene_object obj1 = create("Parent");
         *
         * atlas::scene_object obj2 = create("Chlid");
         *
         * // obj2 is the child of obj1
         * // As obj1 is a parent node
         * obj2.child_of(obj1);
         *
         * ```
         *
         */
        void child_of(const scene_object& p_parent);

        /**
         * @brief iterates through all children entities if the given entity is
         * a parent of any given entities
         *
         *
         * Example Usage:
         *
         * ```C++
         *
         * atlas::scene_object obj1 = create("Parent Node");
         * atlas::scene_object obj2 = create("Chlid Node");
         *
         * // obj1 is the parent of obj2.
         * obj2.child_of(parent);
         *
         * // iteration should only include for "Child Node"
         * obj1.children([](flecs::entity p_child){
         *     // do stuff with the child entity
         * });
         *
         * ```
         *
         */
        template<typename UFunction>
        void children(UFunction&& p_callback) {
            children(p_callback);
        }

        flecs::entity* operator->();
    };

};