#pragma once
#include <core/core.hpp>
#include <variant>
#include <core/scene/components.hpp>
#include <tuple>
#include <flecs.h>

namespace atlas {

    /**
     * @brief scene_object represents a game object that is associated to a
     * given scene
     *
     * Wraps around flecs::entity that provides additional API's to use with the
     * scene object
     *
     * Creating game objects are returned as atlas::strong_ptr to reduce memory
     * unsafe situation with lifetime access violation and nullable objects
     */
    class scene_object {
    public:
        /***
         * @param p_registry is the address to the ecs registry
         * @param p_name is the name supplied to the entity
         */
        scene_object(flecs::world* p_registry, const std::string& p_name);

        /***
         * @param p_registry is the address to the ecs registry
         * @param p_name is the name supplied to the entity
         * @param p_lookup is the tag to indicate lookup for alread-existing
         * entity in flecs::world
         */
        scene_object(flecs::world* p_registry,
                     const std::string& p_name,
                     bool p_lookup);

        /**
         * @param p_registry is a strong_ptr that is expected to be always valid
         * and manages this entity when application closes
         */
        scene_object(strong_ref<flecs::world>& p_registry,
                     const std::string& p_name);

        ~scene_object();

        //! @brief adds a component to the entity
        template<typename UComponent>
        void add() {
            m_entity.add<UComponent>();
        }

        //! @brief adds a component with an assigned value to the entity
        template<typename UComponent>
        void add(UComponent p_component_value) const {
            m_entity.add<UComponent>(p_component_value);
        }

        //! @brief sets the scene object to correspond to a specified parent
        void child_of(strong_ref<scene_object>& p_parent) {
            flecs::entity e = *p_parent;
            m_entity.add(flecs::ChildOf, e);
        }

        //! @brief iterates the entity children entities
        template<typename UFunction>
        void children(UFunction&& p_callback) {
            m_entity.children(p_callback);
        }

        /**
         * @return uint32_t
         *
         * The entity ID
         */
        [[nodiscard]] uint32_t id() const { return m_entity.id(); }

        [[nodiscard]] bool is_alive() const { return m_entity.is_alive(); }

        [[nodiscard]] bool is_valid() const { return m_entity.is_valid(); }

        /**
         * @brief Adds multiple components with no values assigned to them
         * EXPERIEMENTAL: Function to add multiple components but still in its
         * experiemental stages.
         *
         * Example:
         * m_entity_example->add_query<atlas::transform, atlas::rigidbody>();
         */
        template<typename... Args>
        void add_query() {
            using tuple_variadic = std::tuple<Args...>;
            std::variant<tuple_variadic> conditions;
            std::visit(
              [&](const auto& p_component) {
                  std::apply(
                    [&](auto&... p_placeholder) {
                        (m_entity.add<std::decay_t<decltype(p_placeholder)>>(),
                         ...);
                    },
                    p_component);
              },
              conditions);
        }

        //! @return nullptr if entity is not found, otherwise return view of the
        //! component
        template<typename UComponent>
        [[nodiscard]] const UComponent* get() const {
            return m_entity.get<UComponent>();
        }

        //! @return nullptr if entity is not found, otherwise return mutable
        //! component
        template<typename UComponent>
        [[nodiscard]] UComponent* get_mut() {
            return m_entity.get_mut<UComponent>();
        }

        //! @brief is true if component stored in this entity
        template<typename UComponent>
        bool has() {
            return m_entity.has<UComponent>();
        }

        template<typename UComponent>
        void set(const UComponent& p_component) {
            m_entity.set<UComponent>(p_component);
        }

        template<typename UComponent>
        void remove() {
            return m_entity.remove<UComponent>();
        }

        //! @brief Explicitly deleting the entity
        void destruct() const { m_entity.destruct(); }

        operator flecs::entity() const { return m_entity; }

        operator flecs::entity() { return m_entity; }

    private:
        flecs::entity m_entity;
    };
}; // namespace atlas