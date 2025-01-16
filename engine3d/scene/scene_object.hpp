#pragma once
#include <scene/components/components.hpp>
#include <core/engine_logger.hpp>
#include <scene/ecs/entity.hpp>

namespace engine3d{
    /**
     * @name Scene Object
     * @note Defines our object in a given Scene
     * @note Utilizes our wrapper EntityObject that wraps around the flecs API
     * 
     * @note Provides same API as the EntityObject
     * @note While also providing additional API's on how engine3d may define scene objects
    */
    class SceneObject{
    public:
        SceneObject(flecs::world* p_Registry, const std::string& p_Tag) : m_Tag(p_Tag), m_Entity(p_Registry, m_Tag){
            m_Entity.AddComponent<Transform>();
            m_Model = glm::mat4(1.0f);
        }

        SceneObject(const flecs::entity& p_Entity) : m_Entity(p_Entity) {}

        ~SceneObject(){
            ConsoleLogFatal("Scene Object Tag = {} HAS DESTRUCTED!!!", m_Tag);
            if(m_Entity.IsAlive()){
                m_Entity.OnDestruction();
            }
        }

        std::string GetTag() const {
            return m_Tag;
        }

        glm::mat4 GetModelMatrix() const { return m_Model; }

        template<typename UComponent>
        void AddComponent(){
            m_Entity.AddComponent<UComponent>();
        }

        template<typename UComponent>
        void AddComponent(UComponent& p_ComponentValue){
            m_Entity.AddComponent<UComponent>(p_ComponentValue);
        }

        template<typename UComponent>
        const UComponent* GetComponent() const {
            return m_Entity.GetComponent<UComponent>();
        }

        template<typename UComponent>
        UComponent* GetMutableComponent(){
            return m_Entity.GetMutableComponent<UComponent>();
        }

        template<typename UComponent>
        bool HasComponent(){
            return m_Entity.HasComponent<UComponent>();
        }

        template<typename UComponent>
        void SetComponent(const UComponent& p_Component){
            m_Entity.SetComponent<UComponent>(p_Component);
        }

        template<typename UComponent, typename UComponent2>
        void SetComponent(const UComponent& p_Component, const UComponent2& p_Component2){
            m_Entity.SetComponent<UComponent>(p_Component, p_Component2);
        }

        template<typename UComponent>
        void RemoveComponent(){
            return m_Entity.RemoveComponent<UComponent>();
        }

        glm::mat4 GetModelMatrix(){
            const Transform* transform_component = GetComponent<Transform>();

            auto transform = glm::translate(glm::mat4{1.f}, transform_component->Position);

            glm::quat quaterion{
                transform_component->QuaternionRotation.w,
                transform_component->QuaternionRotation.x,
                transform_component->QuaternionRotation.y,
                transform_component->QuaternionRotation.z,
            };

            
            transform *= glm::mat4_cast(quaterion);
            transform = glm::scale(transform, transform_component->Scale);
            // transform = glm::rotate(glm::radians(45.0f), transform_component->Rotation);
            return transform;
        }



    private:
        std::string m_Tag="New Entity";
        EntityObject m_Entity;
        glm::mat4 m_Model;
    };
};