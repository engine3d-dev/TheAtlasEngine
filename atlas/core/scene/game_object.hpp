#pragma once

#include <flecs.h>

namespace atlas {
    class scene_object_exp : public flecs::entity {
    public:
        scene_object_exp() = default;
        scene_object_exp(flecs::world_t* p_registry, flecs::entity_t p_id);

        scene_object_exp(const flecs::entity& p_base);

        explicit scene_object_exp(flecs::entity& p_base);

        flecs::entity* operator->();
    };

    // class scene_object_exp {
    // public:
    //     scene_object_exp() = default;
    //     scene_object_exp(custom_entity& p_entity) : m_entity(&p_entity) {}


    //     custom_entity* operator->() const& {
    //         if(m_entity == nullptr) {
    //             throw std::runtime_error("Invalid access to game object!!");
    //         }

    //         return m_entity;
    //     }

    //     operator custom_entity*() {
    //         return m_entity;
    //     }

    // private:
    //     custom_entity* m_entity;
    // };
};