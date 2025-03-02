#pragma once
#include <core/engine_logger.hpp>
#include <core/scene/components.hpp>
#include <core/scene/entity.hpp>

namespace atlas {
    /**
     * @name Scene Object
     * @note Defines our object in a given Scene
     * @note Utilizes our wrapper entity_t that wraps around the flecs API
     *
     * @note Provides same API as the entity_t
     * @note While also providing additional API's on how engine3d may define
     * scene objects
     */
    class scene_object {
    public:
        scene_object() = default;
        scene_object(flecs::world* p_registry, const std::string& p_tag)
          : m_entity(p_registry, p_tag) {
            //! @note Because flecs the ecs framework uses components to
            //! indicate which entities you want to query from.
            //! @note Each entity the engine creates will by default contain the
            // m_entity.set<Tag>({p_tag});
            // m_entity.add<Transform>();
            m_model = glm::mat4(1.0f);
        }

        scene_object(const flecs::entity& p_Entity)
          : m_entity(p_Entity) {}

        ~scene_object() {
            console_log_fatal("Scene Object Tag = {} HAS DESTRUCTED!!!",
                              m_entity.get<Tag>()->TagMetadata);
            if (m_entity.is_alive()) {
                m_entity.on_destruction();
            }
        }

        std::string get_tag() const { return m_entity.get<Tag>()->TagMetadata; }

        glm::mat4 GetModelMatrix() const { return m_model; }
        void SetModel(const glm::mat4& p_other) { m_model = p_other; }

        template<typename UComponent>
        void add() {
            m_entity.add<UComponent>();
        }

        template<typename UComponent>
        void add(UComponent& p_ComponentValue) {
            m_entity.add<UComponent>(p_ComponentValue);
        }

        template<typename UComponent>
        const UComponent* get() const {
            return m_entity.get<UComponent>();
        }

        template<typename UComponent>
        UComponent* get_mut() {
            return m_entity.get_mut<UComponent>();
        }

        template<typename UComponent>
        bool has() {
            return m_entity.has<UComponent>();
        }

        template<typename UComponent>
        void set(const UComponent& p_Component) {
            m_entity.set<UComponent>(p_Component);
        }

        template<typename UComponent, typename UComponent2>
        void set(const UComponent& p_Component,
                 const UComponent2& p_Component2) {
            m_entity.set<UComponent>(p_Component, p_Component2);
        }

        template<typename UComponent>
        void remove() {
            return m_entity.remove<UComponent>();
        }

        void SetRotation(float p_Angle) { m_angle = glm::radians(p_Angle); }

        glm::mat4 get_model() {
            const Transform* transform_component = get<Transform>();
            m_model = glm::mat4(1.f);

            //! @note Anything vec3 has to be vec4
            m_model = glm::translate(m_model, transform_component->Position);
            m_model = glm::scale(m_model, transform_component->Scale);

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