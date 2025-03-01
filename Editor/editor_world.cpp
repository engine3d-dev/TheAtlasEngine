#include "editor_world.hpp"
#include <core/engine_logger.hpp>


editor_world::editor_world(){
    console_log_fatal("Instantiate Default editor_world!");
}

editor_world::editor_world(const std::string& p_tag) : m_tag(p_tag){
    m_main_world = atlas::world_scope("Main World");

    console_log_trace("m_main_world->get_tag() = {}", m_main_world.get_tag());

    m_first_scene = atlas::create_ref<level_scene>("LevelScene");
    m_main_world.add_scene(m_first_scene);
}

//! TODO: Implement either a scene graph, or decide the data structure to go with in how we want to handle scenes