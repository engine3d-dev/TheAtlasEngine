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

    void scene_object::child_of(const scene_object& p_parent) {
        add(flecs::ChildOf, p_parent);
    }

    flecs::entity* scene_object::operator->() {
        // We want to check if this game object is accessible or else throw an
        // exception if invalid or nullptr
        flecs::entity* e = this;

        // We only want to access this entity if it is alive and valid!
        if (!e->is_valid() and !e->is_alive()) {
            throw invalid_access_exception(
              "Invalid access to an invalid pointer to game object");
        }

        return e;
    }
};