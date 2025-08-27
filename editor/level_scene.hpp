#pragma once
#include <core/core.hpp>
#include <core/scene/scene.hpp>
#include <core/scene/scene_object.hpp>
#include <core/serialize/serializer.hpp>

/**
 * @brief Implementation of a custom scene
 *
 * Represent a scene with an associated game objects that correspond to this
 * game object.
 *
 */

class level_scene : public atlas::scene_scope {
public:
    level_scene(const std::string& p_tag);

    ~level_scene() override {
        console_log_error("LevelScene::~LevelScene Destructed!!!");
    }

    void start();

    void on_update();

    void on_ui_update();

private:
    atlas::serializer m_deserializer_test;
    atlas::optional_ref<atlas::scene_object> m_viking_room;
    atlas::optional_ref<atlas::scene_object> m_cube;
    atlas::optional_ref<atlas::scene_object> m_robot_model;
    atlas::optional_ref<atlas::scene_object> m_platform;
    std::pmr::polymorphic_allocator<> m_allocator;
    atlas::optional_ref<atlas::scene_object> m_camera;
};
