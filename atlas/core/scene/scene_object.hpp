#pragma once
#include <core/engine_logger.hpp>
#include <core/scene/components.hpp>
#include <core/scene/entity.hpp>
#include <variant>

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

        ~scene_object() {
            if (m_entity.is_alive()) {
                m_entity.destruct();
            }
        }

        template<typename UComponent>
        void add() {
            m_entity.add<UComponent>();
        }

        template<typename UComponent>
        void add(UComponent& p_component_value) {
            m_entity.add<UComponent>(p_component_value);
        }

        /**
         * @brief Specify groups of entity signatures to add to the entity
         * @brief signatures being the components
         * EXPERIEMENTAL: Function to add multiple components but still in its
         * experiemental stages.
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

        template<typename UComponent>
        [[nodiscard]] const UComponent* get() const {
            return m_entity.get<UComponent>();
        }

        template<typename UComponent>
        [[nodiscard]] UComponent* get_mut() {
            return m_entity.get_mut<UComponent>();
        }

        template<typename UComponent>
        bool has() {
            return m_entity.has<UComponent>();
        }

        template<typename UComponent>
        void set(const UComponent& p_component) {
            m_entity.set<UComponent>(p_component);
        }

        template<typename UComponent, typename UComponent2>
        void set(const UComponent& p_component,
                 const UComponent2& p_component2) {
            m_entity.set<UComponent>(p_component, p_component2);
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