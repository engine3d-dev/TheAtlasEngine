#include <core/engine_logger.hpp>
#include <core/scene/world.hpp>
#include <core/system_framework/system_registry.hpp>

namespace atlas {
    system_registry* system_registry::s_instance = nullptr;

    system_registry::system_registry(const std::string& p_tag)
      : m_tag(p_tag) {
        console_log_manager::create_new_logger(p_tag);
        s_instance = this;
    }

    system_registry::~system_registry() {
        console_log_fatal("~system_registry called!!!");
    }

    ref<world_scope> system_registry::create_world(const std::string& p_tag) {
        return s_instance->append_world_scope(create_ref<world_scope>(p_tag));
    }

    ref<world_scope> system_registry::get_world(const std::string& p_tag) {
        return s_instance->search_world(p_tag);
    }

    ref<world_scope> system_registry::search_world(const std::string& p_tag) {
        if (m_world_registered.contains(p_tag)) {
            return m_world_registered[p_tag];
        }

        return nullptr;
    }

    void system_registry::append_world(const ref<world_scope>& p_world) {
        if (p_world == nullptr) {
            console_log_fatal("p_world = nullptr!!!");
            return;
        }

        m_world_registered.insert({ p_world->get_tag(), p_world });
        console_log_fatal("After inserting p_world->tag = {}",
                          p_world->get_tag());
    }

    ref<world_scope> system_registry::append_world_scope(
      const ref<world_scope>& p_world) {
        if (p_world == nullptr) {
            return nullptr;
        }

        m_world_registered.insert({ p_world->get_tag(), p_world });
        return m_world_registered[p_world->get_tag()];
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
};