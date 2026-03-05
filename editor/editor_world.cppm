module;

#include <string>

export module editor_world;

import atlas.core.utilities;
import atlas.core.scene.world;
import atlas.core.event;
import atlas.drivers.renderer_system;
import atlas.core.scene.uuid;
import atlas.core.level_streamer;
import atlas.core.scene;
import level_scene;

/**
 * @brief editor_world is where a lot of the editor logic will be handled
 */
export class editor_world final : public atlas::world {
public:
    editor_world(const std::string& p_tag,
                 atlas::event::bus& p_bus,
                 atlas::level_streamer& p_level_streamer)
      : atlas::world(p_tag, p_level_streamer)
      , m_bus(&p_bus) {
        m_bus->create_listener<atlas::event::collision_enter>();
        m_bus->create_listener<atlas::event::collision_persisted>();
        m_bus->create_listener<atlas::event::collision_exit>();
        m_bus->create_immediate_listener<atlas::event::mesh_reload>();


        m_bus->trigger<atlas::event::mesh_reload>(this, &editor_world::reload_mesh);

        // Create defualt scene to level streamer
        // Does polymorphic allocations for these customized scenes
        default_custom_scene<level_scene>("LevelScene", p_bus);

        // Set what our current scene is
        // TODO: Probably have `default_custom_scene<UScene>() set this
        current("LevelScene");

        console_log_error("editor_world initialized successfully!!!");
    }

    ~editor_world() override = default;

    void reload_mesh(atlas::event::mesh_reload&) {
        console_log_info("reload_mesh requested from editor_world!");
    }

private:
    atlas::event::bus* m_bus = nullptr;
};