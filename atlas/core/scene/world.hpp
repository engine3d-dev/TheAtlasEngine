#pragma once
#include <core/core.hpp>
#include <flecs.h>
#include <string>
#include <core/scene/scene.hpp>

namespace atlas {

    /**
     * @brief world_scope -- the world area the manages lifetimes of scenes
     *
     * world_scope are selc-contained which manages the lifetimes of those
     * scenes. ALlows should provide the capabilities of creating, enabling, and
     * disabling scenes that are active.
     *
     * Scenes exist within the scope of a world. Whereas a world also manages
     * the scenes and the objects. Managing where the objects are associated
     * within those scenes.
     */
    class world_scope {
    public:
        world_scope() = delete;
        world_scope(const std::string& p_tag);
        ~world_scope();

        [[nodiscard]] std::string name() const { return m_name; }

        void add_scene(const ref<scene_scope>& p_scene_context);

        template<typename T>
        ref<scene_scope> create_custom_scene(const std::string& p_name) {
            static_assert(
              std::is_base_of_v<scene_scope, T>,
              "Must be a scene that inherits from scene_scope as a base class");
            m_scene_container[p_name] = create_ref<T>(p_name);
            return m_scene_container[p_name];
        }

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