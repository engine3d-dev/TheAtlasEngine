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
        scene_object() = default;
        scene_object(flecs::world* p_registry, const std::string& p_tag)
          : m_entity(p_registry, p_tag) {
            m_model = glm::mat4(1.0f);
        }

        scene_object(const flecs::entity& p_entity)
          : m_entity(p_entity) {}

        ~scene_object() {
            console_log_fatal("Scene Object Tag = {} HAS DESTRUCTED!!!",
                              m_entity.get<tag>()->TagMetadata);
            if (m_entity.is_alive()) {
                m_entity.on_destruction();
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
              [&]([[maybe_unused]] const auto& p_component) {
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

        [[nodiscard]] glm::mat4 get_model() {
            const transform* transform_component = get<transform>();
            m_model = glm::mat4(1.f);

            //! @note Anything vec3 has to be vec4
            m_model = glm::translate(m_model, transform_component->Position);
            m_model = glm::scale(m_model, transform_component->Scale);

            //! @note These are just notes from discussions about mathematical
            //! conversion
            //! TODO: The caveaut of this implicit conversion is, mathematically
            //! un-optimized
            //! @note Unoptimized meaning we are squaring quaternions from mat3
            //! to mat4.
            //! @note Squaring a matrix is n^3 operation. slighly faster because
            //! of laser method.
            auto rotation_mat4 =
              glm::mat4(glm::quat(transform_component->Rotation));
            m_model *= rotation_mat4;

            return m_model;
        }

    private:
        entity_t m_entity;
        glm::mat4 m_model;
        float m_angle = glm::radians(90.0f);
    };
}; // namespace atlas