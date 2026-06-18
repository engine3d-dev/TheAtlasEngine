module;

#include <flecs.h>
#include <string>

export module atlas.core.scene;

import atlas.core.utilities;
import atlas.core.event;
export import :game_object;
export import :uuid;
export import :components;

export namespace atlas {
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
        scene(const std::string& p_name, event::bus& p_bus)
          : m_name(p_name)
          , m_bus(&p_bus) {}
        /**
         * @param p_name is the name given to this scene
         * @param p_bus is the globalized event bus that is given access to the
         * scene to subscribe events to it.
         */
        scene(const std::string& p_name, event::bus& p_bus, uuid p_uuid)
          : m_name(p_name)
          , m_bus(&p_bus)
          , m_scene_uuid(p_uuid) {}

        virtual ~scene() = default;

        /**
         * @brief Retrieves if an entity already exists within the registry,
         * create new entity otherwise
         *
         * @param p_name is a string to set the name of the entity
         */
        game_object entity(std::string_view p_name) {
            return game_object(m_registry.entity(p_name.data()));
        }

        /**
         * @brief Retrieves if an entity already exists within the registry,
         * create new entity otherwise
         *
         * @param p_entity_id is the ID to retrieve an entity if it exists,
         * otherwise returns a new entity.
         */
        game_object entity(uint64_t p_id) {
            return game_object(m_registry.entity(p_id));
        }

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

        template<typename UEvent, typename UObject, typename UCallback>
        void trigger(UObject* p_instance, const UCallback& p_callback) {
            m_bus->trigger<UEvent>(p_instance, p_callback);
        }

        template<typename UEvent>
        void signal(UEvent& p_event) {
            m_bus->signal<UEvent>(p_event);
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
        [[nodiscard]] flecs::query_builder<Comps...> query_builder(
          Args&&... args) const {
            return flecs::query_builder<Comps...>(m_registry,
                                                  std::forward(args)...);
        }

        /**
         * @brief systems in flecs are queries + a given callback
         *
         * Which can be ran manually following up to the `.progress` API
         * function
         *
         * Example Usage:
         *
         * We specify a filter at compile-time. Then anything that matches. We
         * run this callback on that particular filter.
         *
         * This systems filter must be set before the registry is used and not
         * after.
         *
         * ```C++
         * m_scene.system<flecs::pair<tag::editor, projection_view>,
         *   transform,
         *   perspective_camera>()
         *   .each([&](flecs::pair<tag::editor, projection_view> p_pair,
         *   transform& p_transform,
         *   perspective_camera& p_camera) {
         * }
         * ```
         *
         */
        template<typename... Components>
        flecs::system_builder<Components...> system(
          const char* name = nullptr) {
            return flecs::system_builder<Components...>(m_registry, name);
        }


        template<typename... Components>
        flecs::observer_builder<Components...> observer(const char* p_name=nullptr) {
            return flecs::observer_builder<Components...>(m_registry, p_name);
        }

        void progress(float p_delta_time) { m_registry.progress(p_delta_time); }

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
         * atlas::game_object obj1 = scene.create("Parent");
         *
         * atlas::game_object obj2 = scene.create("Chlid");
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
        uint32_t children_count(const game_object& p_parent) {
            return query_builder()
              .with(flecs::ChildOf, p_parent)
              .build()
              .count();
        }

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
        [[nodiscard]] event::bus* event_handle() const { return m_bus; }

        /**
         * @return the unique scene ID
         */
        [[nodiscard]] uint64_t unique_id() const { return m_scene_uuid; }

        void set_environment_map(const std::string& p_filepath) {
            environment environment_mapping = {
                .filepath = p_filepath,
            };

            m_registry.set(environment_mapping);
        }

        template<typename T>
        [[nodiscard]] const T* get() const {
            return m_registry.get<T>();
        }

        /**
         * @brief Requires to return flecs::world is returned by reference to
         * prevent making copies of flecs::world
         */
        operator flecs::world&() { return m_registry; }

    private:
        flecs::world m_registry;
        std::string m_name;
        event::bus* m_bus = nullptr;
        uuid m_scene_uuid;
    };
};