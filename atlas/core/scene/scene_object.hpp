#pragma once
#include <flecs.h>

namespace atlas {

    /**
     * @brief Creates a pointer wrapper which extends capabilities of
     * flecs::entity
     *
     * Provides our own construct of API's that handles any workload around some
     * of the raw flecs API's that can still be used by other flecs API's
     * without completely interacting touching raw flecs API
     */
    class scene_object : public flecs::entity {
    public:
        scene_object() = delete;

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
         * Example Usage:
         * 
         *
         * ```C++
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
         */
        template<typename UFunction>
        void children(UFunction&& p_callback) {
            children(p_callback);
        }

        /**
         * @brief Since we do not have a way to ensure that the access point to
         * this object is invalid, we use the -> operator to ensure the object
         * is valid.
         * 
         * Including cannot overload the `.` operator, to ensure object if it is valid.
         *
         * The `->` operator will throw std::runtime_error exception if object's
         * invalid or return the object
         *
         */
        flecs::entity* operator->();
    };

};