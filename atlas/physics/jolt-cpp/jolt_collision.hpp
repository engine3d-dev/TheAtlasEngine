#pragma once

#include <core/scene/scene.hpp>
#include <physics/jolt-cpp/jolt_contact_listener.hpp>
#include <physics/jolt-cpp/jolt_components.hpp>
namespace atlas::physics {

    class jolt_collision {

    public:
        jolt_collision();
        jolt_collision(flecs::entity_t p_entity_id);

        void collision_added(jolt::contact_event& p_event);
        // void collision_removed();
        // void collison_persisted();

    private:
        virtual void on_collision_added(jolt::contact_event& p_event);
        // virtual void on_collision_persisted() = 0;
        // virtual void on_collision_removed() = 0;

    protected:
        uint64_t m_id;
    };
};