module;

#include <string>

export module editor_world;

import atlas.core.utilities;
import atlas.core.scene.world;
import atlas.core.event;
import atlas.drivers.renderer_system;
import level_scene;

/**
* @brief editor_world is where a lot of the editor logic will be handled
* TODO: The level streamer will be constructed here
*/
export class editor_world : public atlas::world {
public:
    editor_world(const std::string& p_tag,
                 atlas::ref<atlas::renderer_system> p_renderer_instance)
      : atlas::world(p_tag), m_renderer(p_renderer_instance) {
        m_bus.create_listener<atlas::event::collision_enter>();
        m_bus.create_listener<atlas::event::collision_persisted>();
        m_bus.create_listener<atlas::event::collision_exit>();

        atlas::ref<level_scene> first_scene = atlas::create_ref<level_scene>("LevelScene", m_bus);

        m_renderer->current_scene_context(first_scene);
        add_scene(first_scene);

        console_log_error("editor_world initialized successfully!~!!");
    }

    ~editor_world() override = default;

private:
    atlas::event::bus m_bus;
    atlas::ref<atlas::renderer_system> m_renderer;
};