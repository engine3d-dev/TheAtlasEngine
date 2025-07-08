#pragma once
#include <core/core.hpp>
#include <core/scene/scene.hpp>
#include <core/scene/scene_object.hpp>

/**
 * @name LevelScene
 * @note Implementation of our scene
 * @note Meaning this is just the first iteration of how we can look at these
 * scenes and how they can be defined
 * @note Each scene is a type of scene scope, this means that when a player
 * leaves the outer scope of a scene, the player transitions into a different
 * scene
 * @note Either the player transitions into a different scene or hits a
 * invisible wall preventing leaving the scope
 */

class level_scene : public atlas::scene_scope {
public:
    level_scene(const std::string& p_tag);

    ~level_scene() override {
        console_log_error("LevelScene::~LevelScene Destructed!!!");
    }

    void on_update();

    void on_ui_update();

    void on_physics_update();

private:
    atlas::optional_ref<atlas::scene_object> m_viking_room;
    atlas::optional_ref<atlas::scene_object> m_cube;
    atlas::optional_ref<atlas::scene_object> m_robot_model;
    atlas::optional_ref<atlas::scene_object> m_platform;

    bool m_is_simulation_enabled = false;
    atlas::optional_ref<atlas::scene_object> m_camera;
};