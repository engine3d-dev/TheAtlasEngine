#include <core/scene/scene.hpp>
#include <core/scene/world.hpp>
#include <core/system_framework/system_registry.hpp>

namespace atlas {

    world_scope::world_scope(const std::string& p_tag) {
        console_log_info("world_scope::world_scope(std::string Tag) = {}",
                         m_tag);
        console_log_fatal(
          "Just registered world using system_registry::register_to(this)");
        m_tag = p_tag;

        //! @note This means that when someone instantiates a world scope that
        //! gets registers immediately at initialization
        //! @note What we could do is have this be registerd OnLoad(bool) where
        //! the bool lets user define whether to register_to this world
        system_registry::register_to(this);
    }

    /*
        Provides a way of passing scene context
        This is just to test system_registry and how registering worlds/scenes
       would work
        @note Really, this would be providing a way of handling scenes in some
       spatial data structure
    */
    void world_scope::add_scene(const ref<scene_scope>& p_scene_context) {
        m_current_scene = p_scene_context;
        m_scene_manager_queue.push_back(m_current_scene);
    }
};