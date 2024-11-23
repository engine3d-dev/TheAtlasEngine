#include <Core/SceneManagment/Components/GameComponent.hpp>
#include <Core/SceneManagment/SceneObjects/SceneObject.hpp>

namespace engine3d
{
    void GameComponent::SetSceneObjectRef(SceneObject &p_Object)
    {
        m_GameObjectRef = &p_Object;
    }
};
