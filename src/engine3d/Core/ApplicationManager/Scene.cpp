#include <Core/ApplicationManager/Scene.hpp>

namespace engine3d
{
  Scene::~Scene()
  {
    m_SceneRegistry.clear();
  }
};
