module;

#include <string>

export module editor:level_scene;

import atlas.application;
import atlas.core.scene;
import atlas.core.scene.game_object;
import atlas.core.event;
import atlas.core.scene.components;
import atlas.core.utilities;

export class level_scene final : public atlas::scene {
public:
    level_scene(const std::string& p_name, atlas::event::bus& p_bus)
      : atlas::scene(p_name, p_bus) {

        atlas::register_start(this, &level_scene::start);
        atlas::register_physics(this, &level_scene::physics_update);
        atlas::register_update(this, &level_scene::on_update);
    }

    ~level_scene() override = default;

    void start() {
        console_log_info("preload_start invoked!");
    }

    void on_update(float) {
        console_log_info("on_update invoked!");
    }

    void physics_update() {
        console_log_info("physics_update invoked!");
    }
};