#include <core/scene/game_object.hpp>
#include <core/scene/components.hpp>
#include <core/scene/exceptions.hpp>
#include <stdexcept>

namespace atlas {
    scene_object_exp::scene_object_exp(
      flecs::world_t* p_registry,
      flecs::entity_t p_id)
      : flecs::entity(p_registry, p_id) {
        add<transform>();
    }

    scene_object_exp::scene_object_exp(
      const flecs::entity& p_base)
      : flecs::entity(p_base) {
        add<transform>();
    }

    scene_object_exp::scene_object_exp(flecs::entity& p_base) : flecs::entity(p_base) {
        add<transform>();
    }

    flecs::entity* scene_object_exp::operator->() {
        // We want to check if this game object is accessible or else throw an
        // exception if invalid or nullptr
        flecs::entity* e = this;
        if (e == nullptr) {
            throw std::runtime_error(
              "Invalid access to an invalid pointer to game object");
        }

        return e;
    }
};