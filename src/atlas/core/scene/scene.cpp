#include <core/scene/scene.hpp>

namespace atlas {
    scene::scene(const std::string& p_name, event::event_bus& p_bus)
      : m_name(p_name)
      , m_bus(&p_bus) {}

    uscene_object scene::entity(const std::string& p_name) {
        return scene_object(m_registry.entity(p_name.c_str()));
    }
 
    uscene_object scene::entity(uint64_t p_entity_id) {
        return scene_object(m_registry.entity(p_entity_id));
    }

    uint32_t scene::children_count(const scene_object& p_parent) {
        return query_builder().with(flecs::ChildOf, p_parent).build().count();
    }

};