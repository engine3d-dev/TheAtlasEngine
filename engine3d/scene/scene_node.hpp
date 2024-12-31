#pragma once
#include <string>
#include <scene/ecs/registry.hpp>
#include <glm/gtc/quaternion.hpp>
#include <scene/components/components.hpp>

namespace engine3d{
    /**
     * @name SceneNode
     * @note Defines our scene object
     * @note Provides basic layers of API's for creating various scene object's
    */
    class SceneNode{
    public:
        SceneNode() = default;
        /**
         * @param Tag
         * @note Defines the current tag to our object
         * 
         * @param SceneRegistry
         * @note per scene has an entt::registry to register the scene objects and their components
        */
        SceneNode(Registry& p_Registry, const std::string& Tag = "Untitled");

        std::string GetTag() const { return m_Tag; }

        template<typename UComponent, typename... Args>
        void AddComponent(Args&&... args){
            m_ObjectRegistry.OnComponentAdd<UComponent>(m_ObjectEntity, std::forward<Args>(args)...);
            //! @note The idea behind this to is that we can keep data components from action components
            // component.SetReferenceToNode(this);
        }

        template<typename UComponent>
        UComponent& GetComponent(){
            // return m_SceneRegistry.get<UComponent>(m_ObjectEntity);
            return m_ObjectRegistry.GetComponent<UComponent>(m_ObjectEntity);
        }

        template<typename UComponent>
        bool HasComponent(){
            return m_ObjectRegistry.HasComponent<UComponent>(m_ObjectEntity);
        }

        //! @note Enabling us to use this to copy component from when we need to copy context from one scene to the other.
        //! @note TODO: This could be moved outside of SceneNode
        template<typename UComponent>
        static void CopyComponent(entt::registry& p_SceneDstRegistry, entt::registry p_SceneSrcRegistry){
            auto view = p_SceneSrcRegistry.view<UComponent>();

            //! @note Just keeping this here to keep in mind IF when we start thinking of copying scenes
            
            for(auto each_entity : view){
                // uint32_t uuid = p_SceneSrcRegistry.get<UComponent>();
                // entt::entity dstEntityID = p_SceneNodeEntityMap.at(uuid);

                auto& component = p_SceneSrcRegistry.get<UComponent>();
                p_SceneDstRegistry.emplace_or_replace<UComponent>(each_entity);
            }
            
        }

        //! @note Returns our actual entity ID.
        operator uint32_t(){
            return (uint32_t)m_ObjectEntity;
        }

        glm::mat4 toMat4(){
            auto transform_component = GetComponent<Transform>();

            auto transform = glm::translate(glm::mat4{1.f}, transform_component.Position);

            glm::quat quaterion{
                transform_component.QuaternionRotation.w,
                transform_component.QuaternionRotation.x,
                transform_component.QuaternionRotation.y,
                transform_component.QuaternionRotation.z,
            };

            transform *= glm::mat4_cast(quaterion);
            transform = glm::scale(transform, transform_component.Scale);
            return transform;
        }




    private:
        //! @note keep in mind all entt::entity is a uint32_t  that is the entity's ID
        Registry m_ObjectRegistry;
        std::string m_Tag = "Untitled";
        entt::entity m_ObjectEntity{entt::null};
    };
};