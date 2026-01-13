module;

#include <string>
#include <flecs.h>

export module level_scene;

import atlas.core.event.types;
import atlas.core.scene;
import atlas.core.scene.game_object;
import atlas.core.event.bus;
import atlas.core.scene.components;
import atlas.core.utilities.state;

export class level_scene final : public atlas::scene {
public:
    level_scene(const std::string& p_name, atlas::event::bus& p_bus) : atlas::scene(p_name, p_bus) {
        auto editor_camera = entity("Editor Camera");
        editor_camera
        .add<flecs::pair<atlas::tag::editor, atlas::projection_view>>();
        editor_camera.set<atlas::transform>({
        .position = { 3.50f, 4.90f, 36.40f },
        .scale{ 1.f },
        });
        editor_camera.set<atlas::perspective_camera>({
        .plane = { 0.1f, 5000.f },
        .is_active = true,
        .field_of_view = 45.f,
        });

        atlas::game_object bob_object = entity("Bob");

        bob_object.add<atlas::point_light>();

        // @brief For now adding this in
        // because we do not have a way to handle empty scenes
        // so we are adding this in just to have a specific mesh associated with the renderer for the time being, just to make sure it works.
        atlas::game_object viking_room = entity("Viking Room");
        viking_room.add<atlas::tag::serialize>();
        viking_room.set<atlas::transform>({
            .position = { -2.70f, 2.70, -8.30f },
            .rotation = { 2.30f, 95.90f, 91.80f },
            .scale{ 1.f },
        });
        viking_room.set<atlas::mesh_source>({
            .model_path = "assets/models/cube.obj",
            .diffuse = "assets/models/wood.png",
        });

        // viking_room.set<atlas::sphere_collider>({
        // .radius = 1.0f,
        // });

        // viking_room.set<atlas::physics_body>({
        // .friction = 15.f,
        // .restitution = 0.3f,
        // .body_movement_type = atlas::dynamic,
        // });

        // atlas::game_object cube = entity("Aircraft");

        // cube.set<atlas::transform>({
        // .position = { 0.f, 2.10f, -7.30f },
        // .scale = { 0.9f, 0.9f, 0.9f },
        // });

        // cube.set<atlas::mesh_source>({
        // .color = { 1.f, 1.f, 1.f, 1.f },
        // // .model_path = "assets/models/E 45 Aircraft_obj.obj",
        // .model_path = "assets/backpack/backpack.obj",
        // .diffuse = "assets/backpack/diffuse.jpg",
        // .specular = "assets/backpack/specular.jpg"
        // //   .diffuse = "assets/models/E-45-steel detail_2_col.jpg",
        // });

        // atlas::game_object robot_model = entity("Cube");
        // robot_model.add<atlas::tag::serialize>();
        // // robot_model.add<atlas::tag::serialize>();
        // robot_model.set<atlas::transform>({
        // .position = { -2.70, 3.50f, 4.10f },
        // .scale = { 1.f, 1.f, 1.f },
        // });

        // robot_model.set<atlas::mesh_source>(
        // { .color = { 1.f, 1.f, 1.f, 1.f },
        //     .model_path = "assets/models/cube.obj",
        //     .diffuse = "assets/models/container_diffuse.png",
        //     .specular = "assets/models/container_specular.png" });

        // robot_model.set<atlas::box_collider>({
        // .half_extent = { 1.f, 1.f, 1.f },
        // });
        // robot_model.set<atlas::physics_body>({
        // //   .restitution = 1.f,
        // .body_movement_type = atlas::dynamic,
        // });

        // atlas::game_object platform = entity("Platform");

        // platform.set<atlas::transform>({
        // .scale = { 15.f, 0.30f, 10.0f },
        // });
        // platform.set<atlas::mesh_source>({
        // .model_path = "assets/models/cube.obj",
        // .diffuse = "assets/models/wood.png",
        // });
        // platform.set<atlas::physics_body>({
        // .body_movement_type = atlas::fixed,
        // });
        // platform.set<atlas::box_collider>({
        // .half_extent = { 15.f, 0.30f, 10.0f },
        // });

        // atlas::game_object point_light = entity("Point Light 1");
        // point_light.set<atlas::transform>({
        // .position = { 0.f, 2.10f, -7.30f },
        // .scale = { 0.9f, 0.9f, 0.9f },
        // });

        // point_light.set<atlas::mesh_source>({
        // .model_path = "assets/models/cube.obj",
        // .diffuse = "assets/models/wood.png",
        // });
        // point_light.add<atlas::tag::serialize>();

        // benchmark

        // auto start = std::chrono::high_resolution_clock::now();
        // TEMP Code
        // [[maybe_unused]] atlas::game_object point_light_test = entity("Point
        // Light 1"); auto end = std::chrono::high_resolution_clock::now(); auto
        // duration = (end - start);

        // auto seconds =
        // std::chrono::duration_cast<std::chrono::seconds>(duration).count(); auto
        // nanoseconds =
        // std::chrono::duration_cast<std::chrono::nanoseconds>(duration).count();
        // auto microseconds =
        // std::chrono::duration_cast<std::chrono::microseconds>(duration).count();

        // console_log_fatal("Seconds = {:.1f}", static_cast<float>(seconds));
        // console_log_fatal("Nanoseconds = {:.1f}",
        // static_cast<float>(nanoseconds)); console_log_fatal("Microseconds =
        // {:.1f}", static_cast<float>(microseconds));

        // for(size_t i = 0; i < 26; i++) {
        // 	auto obj = entity(std::format("Object #{}", i));
        // 	obj->set<atlas::physics_body>({
        // 		.restitution = 1.25f,
        // 		.body_movement_type = atlas::dynamic,
        // 	});

        // 	obj->set<atlas::sphere_collider>(
        // 		{
        // 		.radius = 1.0f,
        // 	});

        // 	glm::vec3 pos = {float(0*1.4),float(0 * 1.4),float(0 * -3) };

        // 	obj->set<atlas::transform>({
        // 		.position = pos,
        // 		.rotation = {.3f, 0.0f, 0.0f},
        // 	});

        // 	obj->set<atlas::mesh_source>({
        // 		.model_path = "assets/models/Ball OBJ.obj",
        // 		.diffuse = "assets/models/clear.png",
        // 	});
        // }

        // atlas::game_object gerald = entity("Gerald");
        // gerald.add<atlas::point_light>();

        // TODO: Move this outside of level_scene
        // m_deserializer_test = atlas::serializer();

        subscribe<atlas::event::collision_enter>(this,
                                                &level_scene::collision_enter);

        atlas::register_start(this, &level_scene::start);
        atlas::register_physics(this, &level_scene::physics_update);
        atlas::register_update(this, &level_scene::on_update);
        atlas::register_ui(this, &level_scene::on_ui_update);
    }

    ~level_scene() override = default;

    void start() {
    }

    void on_update() {
    }

    void on_ui_update() {
    }

    void physics_update() {
    }

    void runtime_start() {
    }

    void runtime_stop() {
    }

    void reset_objects() {
    }

private:
    void collision_enter(atlas::event::collision_enter& p_event) {
    }

    void collision_persisted(atlas::event::collision_persisted& p_event) {
    }

private:
    // atlas::serializer m_deserializer_test;
    // flecs::entity m_selected_entity;

    // atlas::game_object_optional m_current_entity;
    // float m_movement_speed = 10.f;

    // Setting physics system
    // TODO -- when refactoring this would be at atlas::world level
    // atlas::physics::physics_engine m_physics_engine;

    // bool m_physics_runtime = false;

    // atlas::ui::dockspace m_editor_dockspace;
    // atlas::ui::menu_item m_editor_menu;

    // Note -- Added this temporarily
    // ImFont* m_font;
};