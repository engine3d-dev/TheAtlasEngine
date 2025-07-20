#pragma once
#include <core/core.hpp>
#include <core/scene/scene.hpp>
#include <core/scene/scene_object.hpp>

/**
 * @name level_scene
 *
 * @brief Implementation of our own customized scene
 *
 * level_scene acts as a group that represents an area where a group of objects
 * are associated with.
 */

class level_scene : public atlas::scene_scope {
public:
    level_scene(const std::string& p_tag);

    ~level_scene() override {
        console_log_error("LevelScene::~LevelScene Destructed!!!");
    }

    void on_update();

    void on_ui_update();

private:
    atlas::optional_ref<atlas::scene_object> m_viking_room;
    atlas::optional_ref<atlas::scene_object> m_cube;
    atlas::optional_ref<atlas::scene_object> m_robot_model;
    atlas::optional_ref<atlas::scene_object> m_platform;
    std::pmr::polymorphic_allocator<> m_allocator;
    bool m_is_simulation_enabled = false;
    atlas::optional_ref<atlas::scene_object> m_camera;
};