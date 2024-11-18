#include <Core/SceneManagment/Components/GameComponent.hpp>

namespace engine3d
{
    void GameComponent::SetSceneObjectRef(SceneObject &p_Object)
    {
        m_GameObjectRef = &p_Object;
    }
};
