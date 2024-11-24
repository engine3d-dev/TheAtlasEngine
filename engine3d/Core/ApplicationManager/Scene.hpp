#pragma once
#include <entt/entt.hpp>

namespace engine3d {

  class SceneObject;
  //! @note TODO: Move this into Core/Scene/ directory. Not in Application Manager.
  class Scene {
    public:
      entt::registry m_SceneRegistry;
      ~Scene();

    private:

  };
};
