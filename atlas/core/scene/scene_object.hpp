#pragma once
#include <core/core.hpp>
#include <variant>
#include <core/scene/components.hpp>
#include <tuple>
#include <flecs.h>

namespace atlas {
    /**
     * @name Scene Object
     * @note Defines our object in a given Scene
     * @note Utilizes our wrapper entity_t that wraps around the flecs API
     *
     * @note Provides same API as the entity_t
     * @note While also providing additional API's on how engine3d may define
     * scene objects
     * @note These are just user-defined entities that will be the
     * game-high-level specifics to interact with the ecs framework
     */
    class scene_object {
    public:
        scene_object(flecs::world* p_registry, const std::string& p_name);

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
        void add(UComponent& p_component_value) {
            m_entity.add<UComponent>(p_component_value);
        }

        //! @brief checks for entity lifetime if alive return true
        [[nodiscard]] bool is_alive() const { return m_entity.is_alive(); }

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

        operator flecs::entity() const { return m_entity; }

        operator flecs::entity() { return m_entity; }

    private:
        flecs::entity m_entity;
    };
}; // namespace atlas