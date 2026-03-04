module;

#include <memory_resource>
#include <memory>
#include <string>
#include <optional>

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
* TODO: The level streamer will be constructed here
*/
export class editor_world final : public atlas::world {
public:
    editor_world(const std::string& p_tag, atlas::event::bus& p_bus,
                 atlas::ref<atlas::renderer_system> p_renderer_instance, atlas::level_streamer& p_level_streamer)
      : atlas::world(p_tag, p_level_streamer), m_bus(&p_bus), m_renderer(std::move(p_renderer_instance)) {
        m_bus->create_listener<atlas::event::collision_enter>();
        m_bus->create_listener<atlas::event::collision_persisted>();
        m_bus->create_listener<atlas::event::collision_exit>();

        // 0.) Add scene to level streamer
        // Does polymorphic allocations for these customized scenes
        default_custom_scene<level_scene>("LevelScene", p_bus);
        
        // 1.) Setting our current scene
        // TODO: Probably have `default_custom_scene<UScene>() set this
        std::string current_scene_name = "LevelScene";
        current(current_scene_name);

        // 2.) Getting current scene to start rendering
        // atlas::ref<atlas::scene> first_scene = current();
        atlas::ref<atlas::scene> first_scene = p_level_streamer.current_scene(current_scene_name);

        // 3.) Render our actual 3D scene
        // TODO: Probably move the atlas::renderer_system to application
        // TODO: Have current() handle the system specification + editor camera usage.
        m_renderer->current_scene_context(first_scene);

        console_log_error("editor_world initialized successfully!!!");
    }

    ~editor_world() override = default;

private:
    atlas::event::bus* m_bus=nullptr;
    atlas::ref<atlas::renderer_system> m_renderer;
};