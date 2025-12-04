#pragma once
#include <flecs.h>
#include <optional>

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
        //! @brief Should not construct a scene object not created through flecs::world
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
         * atlas::uscene_object obj1 = entity("Parent");
         *
         * atlas::uscene_object obj2 = entity("Chlid");
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
         * atlas::uscene_object obj1 = entity("Parent Node");
         * atlas::uscene_object obj2 = entity("Chlid Node");
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
    };

    /**
     * @brief game object type exposed to the user as an alias to game objects
    */
    using uscene_object = std::optional<scene_object>;
};