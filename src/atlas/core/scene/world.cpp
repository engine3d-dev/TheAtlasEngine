#include <core/scene/scene.hpp>
#include <core/scene/world.hpp>
#include <core/system/registry.hpp>

namespace atlas {

    world::world(const std::string& p_name)
      : m_name(p_name) {}

    /**
     * @brief Currently how to pass in the scene context to the world
     * TODO: Have a way of allowing creation and management of those
     * created-scenes be done through world
     */
    void world::add_scene(const ref<scene>& p_scene_context) {
        m_scene_container.emplace(p_scene_context->name(), p_scene_context);
    }
};