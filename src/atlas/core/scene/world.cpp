#include <core/scene/scene.hpp>
#include <core/scene/world.hpp>
#include <core/system/registry.hpp>

namespace atlas {

    world_scope::world_scope(const std::string& p_name)
      : m_name(p_name) {
    }

    world_scope::~world_scope() {
        console_log_trace("~world_scope called!!!");
    }

    /**
     * @brief Currently how to pass in the scene context to the world
     * TODO: Have a way of allowing creation and management of those
     * created-scenes be done through world_scope
     */
    void world_scope::add_scene(const ref<scene_scope>& p_scene_context) {
        m_scene_container.emplace(p_scene_context->name(), p_scene_context);
    }
};