module;

#include <flecs.h>
#include <optional>

export module atlas.core.scene.game_object;
import atlas.core.scene.components;

export namespace atlas {
    /**
     * @brief Creates a pointer wrapper which extends capabilities of
     * flecs::entity
     *
     * Provides our own construct of API's that handles any workload around some
     * of the raw flecs API's that can still be used by other flecs API's
     * without completely interacting touching raw flecs API
     */
    class game_object : public flecs::entity {
    public:
        //! @brief Should not construct a scene object not created through
        //! flecs::world
        game_object() = delete;

        game_object(flecs::world_t* p_registry, flecs::entity_t p_id) : flecs::entity(p_registry, p_id) {
            add<transform>();
        }

        game_object(const flecs::entity& p_base) : flecs::entity(p_base) {
            add<transform>();
        }

        explicit game_object(flecs::entity& p_base) : flecs::entity(p_base) {
            add<transform>();
        }

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
         * atlas::game_object obj1 = entity("Parent");
         *
         * atlas::game_object obj2 = entity("Chlid");
         *
         * // obj2 is the child of obj1
         * // As obj1 is a parent node
         * obj2.child_of(obj1);
         *
         * ```
         *
         */
        void child_of(const std::optional<game_object>& p_parent) {
            add(flecs::ChildOf, p_parent.value());
        }

        /**
         * @brief iterates through all children entities if the given entity is
         * a parent of any given entities
         *
         * Example Usage:
         *
         *
         * ```C++
         * atlas::game_object obj1 = entity("Parent Node");
         * atlas::game_object obj2 = entity("Chlid Node");
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
     * @brief Alias to std::optional<game_object>
     *
     * This alias serves as a representation of game objects users can create
     * and manage components with
     */
    using game_object_optional = std::optional<game_object>;
};