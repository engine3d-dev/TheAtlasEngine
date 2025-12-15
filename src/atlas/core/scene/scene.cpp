#include <core/scene/scene.hpp>

namespace atlas {
    scene::scene(const std::string& p_name, event::event_bus& p_bus)
      : m_name(p_name)
      , m_bus(&p_bus) {}

    game_object scene::entity(std::string_view p_name) {
        return game_object(m_registry.entity(p_name.data()));
    }
        
    game_object scene::entity(uint64_t p_id) {
        return game_object(m_registry.entity(p_id));
    }

    uint32_t scene::children_count(const game_object& p_parent) {
        return query_builder()
          .with(flecs::ChildOf, p_parent)
          .build()
          .count();
    }

};