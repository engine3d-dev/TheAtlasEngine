#pragma once
#include <core/core.hpp>
#include <core/scene/scene.hpp>
#include <core/scene/scene_object.hpp>
#include <core/serialize/serializer.hpp>
#include <imgui.h>
#include <physics/physics_engine.hpp>
#include <core/event/event_bus.hpp>
#include <core/event/types.hpp>
#include <core/editor/dockspace.hpp>
#include <core/editor/menu_item.hpp>

/**
 * @brief Implementation of a custom scene
 *
 * Represent a scene with an associated game objects that correspond to this
 * game object.
 *
 */

class level_scene final : public atlas::scene {
public:
    level_scene(const std::string& p_tag, atlas::event::event_bus& p_bus);

    ~level_scene() override = default;

    void start();

    void on_update();

    void on_ui_update();

    void physics_update();

    void runtime_start();

    void runtime_stop();

    void reset_objects();

private:
    void collision_enter(atlas::event::collision_enter& p_event);

    void collision_persisted(atlas::event::collision_persisted& p_event);

private:
    bool m_blink = false;
    atlas::serializer m_deserializer_test;
    flecs::entity m_selected_entity;

    // atlas::game_object m_viking_room;
    // atlas::game_object m_cube;
    // atlas::game_object m_robot_model;
    // atlas::game_object m_platform;
    // atlas::game_object m_camera;
    // atlas::game_object m_point_light;
    // atlas::game_object m_bob_object;
    atlas::game_object m_current_entity;
    float m_movement_speed = 10.f;
    // std::vector<atlas::strong_ref<atlas::scene_object>> m_many_objects;

    // Setting physics system
    // TODO -- when refactoring this would be at atlas::world layer
    atlas::physics::physics_engine m_physics_engine;

    bool m_physics_runtime = false;

    atlas::ui::dockspace m_editor_dockspace;
    atlas::ui::menu_item m_editor_menu;

    // Note -- Added this temporarily
    // ImFont* m_font;
};
