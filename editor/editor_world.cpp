#include "editor_world.hpp"
#include <core/engine_logger.hpp>
#include <core/system_framework/system_registry.hpp>

editor_world::editor_world() {
    console_log_fatal("Instantiate Default editor_world!");
}

editor_world::editor_world(const std::string& p_tag) {
    m_main_world = atlas::system_registry::create_world(p_tag);

    console_log_trace("m_main_world->get_tag() = {}", m_main_world->name());

    m_first_scene = atlas::create_ref<level_scene>("LevelScene");
    m_main_world->add_scene(m_first_scene);
}