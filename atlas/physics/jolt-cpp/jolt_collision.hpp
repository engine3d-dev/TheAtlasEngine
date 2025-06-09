#pragma once

#include <core/scene/scene.hpp>
#include <physics/jolt-cpp/jolt_contact_listener.hpp>
#include <physics/jolt-cpp/jolt_components.hpp>
namespace atlas::physics {
    /**
     * @brief This is the collision api. It allows users to create children of
     * this collision handler and write their own functions over the virtual
     * ones. Then they can assign it to the engine.
     *
     */
    class jolt_collision {

    public:
        jolt_collision();
        jolt_collision(flecs::entity_t p_entity_id);

        void collision_added(contact_event& p_event);
        //! @note FIXME: Add these function to handle removal and continous
        //! collision detection:

        // void collision_removed();
        // void collison_persisted();

    private:
        virtual void on_collision_added(contact_event& p_event);
        //! @note FIXME: Add these function to handle removal and continous
        //! collision detection:

        // virtual void on_collision_persisted() = 0;
        // virtual void on_collision_removed() = 0;

    protected:
        /**
         * @brief Allows to keep track of the collision events and what that are
         * assigned to.
         *
         */
        uint64_t m_id;
    };
};