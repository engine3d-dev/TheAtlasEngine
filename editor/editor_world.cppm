module;

#include <string>

export module editor_world;

import atlas.core.utilities;
import atlas.core.scene.world;
import atlas.core.event;
import atlas.core.scene.system_registry;
import atlas.drivers.renderer_system;
import level_scene;

export class editor_world {
public:
    editor_world(const std::string& p_tag,
                 atlas::ref<atlas::renderer_system> p_renderer_instance)
      : m_renderer(p_renderer_instance) {
        // m_main_world = atlas::system_registry::create_world(p_tag);
        m_main_world = atlas::create_ref<atlas::world>(p_tag);

        m_bus.create_listener<atlas::event::collision_enter>();
        m_bus.create_listener<atlas::event::collision_persisted>();
        m_bus.create_listener<atlas::event::collision_exit>();

        atlas::ref<level_scene> first_scene = atlas::create_ref<level_scene>("LevelScene", m_bus);

        m_renderer->current_scene_context(first_scene);
        m_main_world->add_scene(first_scene);

        console_log_error("editor_world initialized successfully!~!!");
    }

    atlas::ref<atlas::world> world() { return m_main_world; }

private:
    atlas::ref<atlas::world> m_main_world;

    atlas::event::bus m_bus;
    atlas::ref<atlas::renderer_system> m_renderer;
};