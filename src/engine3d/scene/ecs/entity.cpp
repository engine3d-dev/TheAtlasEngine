#include <scene/components/components.hpp>
#include <scene/ecs/entity.hpp>

namespace engine3d{
    EntityObject::EntityObject(flecs::world* p_SceneRegistryToWorld, const std::string& p_Tag){
        m_EntityID = p_SceneRegistryToWorld->entity(p_Tag.c_str());
        
        //! @note These are default components the entity will store that all game objects SHOULD have by default.
        AddComponent<Transform>();
    }

    EntityObject::~EntityObject(){
        //! @note We make sure that if this entity is alive we can destruct it
        //! @note Destructor will only be called if the object goes out of scope
        if(this->IsAlive()){
            OnDestruction();
        }
    }
};