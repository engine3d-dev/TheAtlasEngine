#pragma once

#include <Core\SceneManagment\SceneObjects\SceneObject.hpp>
#include <cstdint>

namespace engine3d 
{
  //! @note Remove GameComponent
  //! @note Should be handled by SceneNode
  class GameComponent
  {
    public:
      GameComponent() = default;
      ~GameComponent() = default;

      //! @note Remove These functions
      void SetSceneObjectRef(SceneObject &p_Object);
      void SetComponentID(uint64_t p_CompID);

    protected:
      //! @note m_GameObjectRef will be how the SceneNode references to its parent.
      //! @note Each scene object will have a UUID for per scene object
      SceneObject *m_GameObjectRef;
      uint64_t m_ComponentID;
  };
}; // namespace Engine3D
