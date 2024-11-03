#pragma once

#include <Core/SceneManagment/SceneObjects/SceneObject.hpp>
#include <cstdint>

namespace Engine3D 
{
  class GameComponent
  {
    public:
      GameComponent();
      ~GameComponent();

      void SetSceneObjectRef(SceneObject &p_Object);
      void SetComponentID(uint64_t p_CompID);

    private:
      SceneObject *m_GameObjectRef;
      uint64_t m_ComponentID;
  };
}; // namespace Engine3D
