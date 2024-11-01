#include <Core/SceneManagment/Components/GameComponent.hpp>

namespace Engine3D
{
    void GameComponent::SetSceneObjectRef(SceneObject& p_Object)
    {
        m_GameObjectRef = &p_Object;
    }
};
