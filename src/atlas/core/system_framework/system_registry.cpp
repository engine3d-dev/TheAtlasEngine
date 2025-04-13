#include <core/engine_logger.hpp>
#include <core/scene/world.hpp>
#include <core/system_framework/system_registry.hpp>

namespace atlas {
    system_registry* system_registry::s_instance = nullptr;
    static std::deque<world_scope> g_initial_worlds;

    //! @note At initialization what should system registry know?
    //! @note How do we assign a hash as part of the tag?

    system_registry::system_registry(const std::string& p_tag) : m_tag(p_tag){
        console_log_manager::create_new_logger(p_tag);
        s_instance = this;
    }

    ref<world_scope> system_registry::create_world(const std::string& p_tag) {
        // return create_ref<world_scope>(p_tag);
        // m_world_registered[p_tag] = create_ref<world_scope>(p_tag);
        // m_world_registered.insert({p_tag, world_scope(p_tag)});
        return s_instance->append_world_scope(create_ref<world_scope>(p_tag));
    }

    //! @note TODO: Probably want to have g_CurrentWorld in a deque of some sort
    //! @note We need to ensure that a world scope is in synced if there are
    //! multiple worlds
    void system_registry::register_to(const ref<world_scope>& p_world) {
        // g_initial_worlds.push_back(*p_world);

        s_instance->append_world(p_world);

        // auto current_world = *g_initial_worlds.begin();
        

        console_log_info("register_to::p_World->get_tag() = {}", p_world->get_tag());
    }

    ref<world_scope> system_registry::get_world(const std::string& p_tag) {
        return s_instance->search_world(p_tag);
    }

    ref<world_scope> system_registry::search_world(const std::string& p_tag) {
        if(m_world_registered.contains(p_tag)) {
            return m_world_registered[p_tag];
        }

        return nullptr;
    }

    /*
        Here is an idea we can use world_scope for

        world_scope
            - Contains UUID(hash) associated with it
            - All scenes within this world scope contains that has
                Example: scene_scope(Tag: "SceneName#1234")
                         Where name of the scene is "SceneName", world scope
       hash is "1234" as the initial hash
            - When serializing the UUID gets re-generated every-time

        scene_scope
            - Contains its initial tag and the hash of the world it is
       associated with
            - How the scene_scope gets to know the world its associated is when
       you register_to the scene doing, system_registry::register_to(this)
            - This can be throw some API like, system_registry::get_world(this),
       where we fetch the scene scope's hash UUID
            - Using this information to getting the world scope with our scene
    */
    world_scope system_registry::get_world() {
        return *g_initial_worlds.begin();
    }
};