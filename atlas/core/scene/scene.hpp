#pragma once
#include <core/core.hpp>
#include <string>
#include <core/event/event_bus.hpp>
#include <core/scene/scene_object.hpp>

namespace atlas {

    /**
     * @brief Constructs a scene that defines an area where game objects are
     * part of contained within an atlas::world
     *
     * Scenes are fundamentally building blocks that a game object may reside
     * in. Composing of variety of properties such as sky lights, game objects,
     * cameras, etc.
     *
     * Which can represent corresponding resources that can be related to
     * specific levels/scenes.
     *
     * Scenes are apart of another container referred to as the world which
     * helps manages these scenes and consider them as contexts that can be
     * switched/toggled based on users transforms.
     */
    class scene {
    public:
        /**
         * @param p_name is the name given to this scene
         * @param p_bus is the globalized event bus that is given access to the
         * scene to subscribe events to it.
         */
        scene(const std::string& p_name, event::event_bus& p_bus);

        virtual ~scene() = default;

        /**
         * @brief Retrieves if an entity already exists within the registry,
         * create new entity otherwise
         *
         * @param p_name is a string to set the name of the entity
         */
        game_object entity(const std::string& p_name);

        /**
         * @brief Retrieves if an entity already exists within the registry,
         * create new entity otherwise
         *
         * @param p_entity_id is the ID to retrieve an entity if it exists,
         * otherwise returns a new entity.
         */
        game_object entity(uint64_t p_entity_id);

        /**
         * @brief subscribes an event to the event::bus to get invoked when
         * publishers notify all subscribers when an update is incoming
         *
         * @tparam UEventType
         * @tparam UObject
         * @tparam UCallback
         * @param p_instance is the object instance to specify where the
         * callback belongs to
         * @param p_callback is the callback that contains an arbitrary task
         * that gets invoked when incoming updates occur from the publisher
         *
         */
        template<typename UEventType, typename UObject, typename UCallback>
        void subscribe(UObject* p_instance, const UCallback& p_callback) {
            m_bus->subscribe<UEventType>(p_instance, p_callback);
        }

        /**
         * @brief queries components, returning entities (game objects) that
         * contain those components queried with.
         *
         * Queries are a feature flecs allows us to do. Which query component
         * parameters and gives us archetypes back.
         *
         * Archetypes are list of entities that contain those list of parameters
         *
         * @tparam ...Comps
         * @tparam ...Args
         * @param ... args
         * @return flecs::query_builder<Comps...>
         *
         * Example Usage:
         * ```C++
         * auto query_physics = query_builder<atlas::transform,
         * atlas::physics_body>().build();
         *
         * query_physics.each([](flecs::entity, atlas::transform&,
         * atlas::physics_body&){
         *      // do stuff
         * });
         * ```
         */
        template<typename... Comps, typename... Args>
        flecs::query_builder<Comps...> query_builder(Args&&... args) const {
            return flecs::query_builder<Comps...>(m_registry,
                                                  std::forward(args)...);
        }

        /**
         * @return the number of children entities
         *
         * Example Usage:
         *
         *
         * ```C++
         *
         * atlas::scene scene("New Scene");
         *
         * // creating obj1 (parent) and obj2 (child)
         * atlas::scene_object obj1 = scene.create("Parent");
         *
         * atlas::scene_object obj2 = scene.create("Chlid");
         *
         * // obj2 is the child of obj1
         * // As obj1 is a parent node
         *
         * obj2.child_of(obj1);
         *
         * // Returns 1
         * uint32_t obj1_children = scene.children_count(obj1);
         *
         * ```
         */
        uint32_t children_count(const game_object& p_parent);

        /**
         * @brief Defer operations until end of frame.
         * When this operation is invoked while iterating, operations
         * inbetween the defer_begin() and defer_end() operations are executed
         * at the end of the frame.
         *
         * This operation is thread safe.
         *
         * @return true if world changed from non-deferred mode to deferred
         * mode.
         */
        bool defer_begin() { return m_registry.defer_begin(); }

        /**
         * End block of operations to defer.
         * See defer_begin().
         *
         * This operation is thread safe.
         *
         * @return true if world changed from deferred mode to non-deferred
         * mode.
         */
        bool defer_end() { return m_registry.defer_end(); }

        //! @return the name of the scene
        [[nodiscard]] std::string name() const { return m_name; }

        //! @return the event::bus handle for subscribing events
        [[nodiscard]] event::event_bus* event_handle() const { return m_bus; }

        /**
         * @brief Requires to return flecs::world is returned by reference to
         * prevent making copies of flecs::world
         */
        operator flecs::world&() { return m_registry; }

    private:
        flecs::world m_registry;
        std::string m_name;
        event::event_bus* m_bus = nullptr;
    };
}; // namespace atlas