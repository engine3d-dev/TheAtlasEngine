#include <core/common.hpp>
#include <core/scene/scene_object.hpp>
#include <core/scene/components.hpp>
#include <core/scene/exceptions.hpp>

namespace atlas {
    scene_object::scene_object(flecs::world_t* p_registry, flecs::entity_t p_id)
      : flecs::entity(p_registry, p_id) {
        add<transform>();
    }

    scene_object::scene_object(const flecs::entity& p_base)
      : flecs::entity(p_base) {
        add<transform>();
    }

    scene_object::scene_object(flecs::entity& p_base)
      : flecs::entity(p_base) {
        add<transform>();
    }

    void scene_object::child_of(const std::optional<scene_object>& p_parent) {
        add(flecs::ChildOf, p_parent.value());
    }
};