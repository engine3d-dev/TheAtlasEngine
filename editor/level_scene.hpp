#pragma once
#include <core/scene/scene.hpp>
#include <core/scene/scene_object.hpp>
#include <map>
#include <physics/physics_scene.hpp>

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
    level_scene();

    level_scene(const std::string& p_tag);

    virtual ~level_scene() {
        console_log_error("LevelScene::~LevelScene Destructed!!!");
    }

    //! @note Remove these from being overridden
    //! TODO: We should have an indication on what functions are update phased
    //! functions
    void on_update();

    void on_ui_update();

    //! TODO: Replacing this with batch rendering
    //! @note Typically end_frame would handle this
    //! TODDO: Remove this function. Remove the subscribe for the function.
    //! TODO: Provide ways of submitting scene objects from a scene to the
    //! rendderer. Rather having the scene renderer deal with that
    void on_scene_render();

    //! TODO: Remove this and integrate a few functions into LevelScene such as
    //! on_runtime_start/on_runtime_stop
    void on_physics_update();

    void on_runtime_start();
    void on_runtime_stop();

private:
    atlas::ref<atlas::scene_object> m_sphere;
    atlas::ref<atlas::scene_object> m_platform;
    bool m_is_simulation_enabled = false;
    atlas::ref<atlas::scene_object> m_camera;
    atlas::physics::physics_scene m_physics_scene;
};