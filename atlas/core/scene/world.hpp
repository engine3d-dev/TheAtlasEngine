#pragma once
#include <map>
#include <string>
#include <core/core.hpp>
#include <flecs.h>
#include <core/scene/scene.hpp>

namespace atlas {

    /**
     * @brief world represents a larger scope of areas that manages the scene
     * contexts
     *
     * The world is responsible for providing a self-containment of scenes and
     * managing which scenes are current. Based on current scenes are details
     * the world can know about.
     *
     * Details such as through player transform or navigation that can be done
     * through the world, providing useful information on where a game object in
     * those scenes/levels.
     *
     * The world responsibility act as the based container of any scene that can
     * be created by the user through editor.
     *
     *
     */
    class world_scope {
    public:
        world_scope() = delete;

        /**
         * @brief construct a new world_scope with a specified name associated
         * with it
         */
        world_scope(const std::string& p_name);
        ~world_scope();

        //! @return the name of world_scope
        [[nodiscard]] std::string name() const { return m_name; }

        /**
         * @brief Creating a scene, then we add that scene onto this world
         *
         * @warning TODO: This is not how we want to currently create and add
         * scenes as this is quite problematic. Should direct attention to this
         * soon.
         */
        void add_scene(const ref<scene_scope>& p_scene_context);

        template<typename T>
        ref<scene_scope> create_custom_scene(const std::string& p_name) {
            static_assert(
              std::is_base_of_v<scene_scope, T>,
              "Must be a scene that inherits from scene_scope as a base class");
            m_scene_container[p_name] = create_ref<T>(p_name);
            return m_scene_container[p_name];
        }

        /**
         * @brief get_scene allows for specifically querying for current scenes
         *
         * TODO: Eventually this is going to be something defined whether its
         * through the player's navigation system for the world to track where
         * the player is in within the world, then provide the current scene
         * based on that information
         */
        ref<scene_scope> get_scene(const std::string& p_tag) {
            if (!m_scene_container.contains(p_tag)) {
                throw std::runtime_error(
                  "Could not access ref<scene_scope> from "
                  "world_scope::get_scene(const string& p_tag)!!!");
            }
            return m_scene_container[p_tag];
        }

    private:
        std::map<std::string, ref<scene_scope>> m_scene_container;
        ref<world_scope> m_world_shared_instance;
        std::string m_name = "Undefined Tag";
    };
}; // namespace atlas