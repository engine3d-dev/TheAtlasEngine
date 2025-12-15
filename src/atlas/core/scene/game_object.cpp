#include <core/common.hpp>
#include <core/scene/game_object.hpp>
#include <core/scene/components.hpp>
#include <core/scene/exceptions.hpp>

namespace atlas {
    game_object::game_object(flecs::world_t* p_registry, flecs::entity_t p_id)
      : flecs::entity(p_registry, p_id) {
        add<transform>();
    }

    game_object::game_object(const flecs::entity& p_base)
      : flecs::entity(p_base) {
        add<transform>();
    }

    game_object::game_object(flecs::entity& p_base)
      : flecs::entity(p_base) {
        add<transform>();
    }

    void game_object::child_of(const std::optional<game_object>& p_parent) {
        add(flecs::ChildOf, p_parent.value());
    }
};