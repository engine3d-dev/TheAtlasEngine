#pragma once
#include <flecs.h>
#include <string>
#include <core/scene/world.hpp>
#include <core/system_framework/system_registry.hpp>

namespace engine3d{
    /**
     * @name Entity
     * @note Wrapper around specifically creating a type of entity wrapper around the flecs API
     * @note It provides the same mechanism for adding, removing, and checking if components are stored.
     * @note Flecs API specified that querying components are how they find/iterate through entities
     *       - Queries are also used throughout parts of the API such as their systems and observers
    */
    class EntityObject{
    public:
        EntityObject() = default;
        EntityObject(flecs::world* p_SceneRegistryToWorld, const std::string& p_Tag);
        EntityObject(const flecs::entity& p_Entity) : m_EntityID(p_Entity){}

        ~EntityObject();

        //! @note Flecs allows to check when an entity has been created
        //! @note Knowing when this entity lifecyle's ended
        bool IsAlive() const { return m_EntityID.is_alive(); }

        void OnDestruction(){
            m_EntityID.destruct();
        }

        //! @note Creates component in the ECS storage but does not assign it with a value.
        template<typename UComponent>
        void AddComponent(){
            m_EntityID.add<UComponent>();
        }

        template<typename UComponent>
        void AddComponent(const UComponent& p_ComponentValue){
            m_EntityID.set<UComponent>(p_ComponentValue);
        }

        //! @note Returns the component from the entity
        //! @note Flecs has you return a pointer for checking if the component is found
        //! @note If not found will return nullptr
        template<typename UComponent>
        const UComponent* GetComponent() const{
            return m_EntityID.get<UComponent>();
        }

        template<typename UComponent>
        UComponent* GetMutableComponent(){
            if(HasComponent<UComponent>()){
                ConsoleLogTrace("IT DOES contain MeshComponent!");
                // return nullptr;
            }
            ConsoleLogWarn("GetMutableComponent Called from entity.hpp with World Tag = {}", SystemRegistry::GetWorld().GetTag());
            // return SystemRegistry::GetWorld()->GetRegistry().get_mut<UComponent>(m_EntityID);
            return SystemRegistry::GetWorld().GetRegistry()->get_mut<UComponent>(m_EntityID);
        }

        //! @note Checks if specific component of type UComponent is provided to this entity
        template<typename UComponent>
        bool HasComponent(){
            return m_EntityID.has<UComponent>();
        }

        //! @note Setting a component
        template<typename UComponent>
        void SetComponent(const UComponent& p_Component){
            m_EntityID.set<UComponent>(p_Component);
        }

        //! @note Set value for position and velocity component
        //! @note Flecs state component will be added if entity doesn't have any.
        template<typename T, typename U>
        void SetComponent(const T& p_Component1, const U& p_Component2){
            //! @note When setting multiple components this is how flecs does it typically
            //! @note flecs::entity::set returns a const entity& to set multiple components
            m_EntityID.set<T, U>(p_Component1).set(p_Component2);
        }

        template<typename UComponent>
        void RemoveComponent(){
            m_EntityID.remove<UComponent>();
        }

        operator flecs::entity(){
            return m_EntityID;
        }


    private:
        // Keep in mind that an entity just represent a 64-bit number
        flecs::entity m_EntityID;
    };
};