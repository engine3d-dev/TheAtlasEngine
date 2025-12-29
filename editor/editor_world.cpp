#include "editor_world.hpp"
#include <core/engine_logger.hpp>
#include <core/system/registry.hpp>
#include "level_scene.hpp"

editor_world::editor_world(
  const std::string& p_tag,
  /*NOLINT*/ atlas::ref<atlas::renderer> p_renderer_instance)
  : m_renderer(/*NOLINT*/ p_renderer_instance) {
    m_main_world = atlas::system_registry::create_world(p_tag);

    m_bus.create_listener<atlas::event::collision_enter>();
    m_bus.create_listener<atlas::event::collision_persisted>();
    m_bus.create_listener<atlas::event::collision_exit>();

    atlas::ref<level_scene> first_scene =
      atlas::create_ref<level_scene>("LevelScene", m_bus);
    m_renderer->current_scene(first_scene);
    m_main_world->add_scene(first_scene);
}