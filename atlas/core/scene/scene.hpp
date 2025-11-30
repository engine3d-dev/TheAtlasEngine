#pragma once
#include <core/core.hpp>
#include <core/engine_logger.hpp>
#include <core/scene/scene_object.hpp>
#include <string>
#include <core/scene/types.hpp>
#include <core/event/event_bus.hpp>
#include <core/scene/game_object.hpp>

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
         * @brief Used to creating a game object
         *
         * @param p_name is specified when creating or searching the object
         *
         * @return strong_ptr<atlas::scene_object>
         */
        // strong_ref<scene_object> create_object(const std::string& p_name);

        scene_object_exp create(const std::string& p_name);



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
         * @brief Defer operations until end of frame.
         * When this operation is invoked while iterating, operations inbetween
         * the defer_begin() and defer_end() operations are executed at the end
         * of the frame.
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

        //! @return the name of atlas::scene_object
        [[nodiscard]] std::string name() const { return m_name; }

        //! @return the event::bus handle to do the subscription operation of
        //! events
        [[nodiscard]] event::event_bus* event_handle() const { return m_bus; }

        /**
         * @brief Requires to return flecs::world is returned by reference to
         * prevent making copies of flecs::world
         */
        operator world&() { return m_registry; }

    private:
        // std::pmr::polymorphic_allocator<> m_allocator;
        world m_registry;
        std::string m_name;
        event::event_bus* m_bus = nullptr;
    };
}; // namespace atlas