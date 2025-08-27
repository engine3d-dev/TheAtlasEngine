#pragma once

#include <Jolt/Jolt.h>
#include <Jolt/Physics/Collision/ContactListener.h>
#include <physics/jolt-cpp/jolt_components.hpp>
#include <core/scene/scene.hpp>

namespace atlas::physics {
    /**
     * @brief This is the glue between contact events and jolts contact
     * listener. It takes the calls from jolt through virtual functions and
     * allows users to create children for jolt collision.
     *
     */
    class contact_listener : public JPH::ContactListener {
    private:
        /**
         * @brief This allows us to make sure that the contacts are valid and
         * not something that went wrong. It plays the role of both sanity check
         * and saftey gaurd since if collisions were to be wrong you would not
         * want that to crash the whole game.
         *
         * @param in_body1 This is the object that called it
         * @param in_body2 This is the object that body1 collided with
         * @param in_base_offset This tells how apart the center of the objetcs
         * are
         * @param in_collision_result The details about the collision
         * @return JPH::ValidateResult
         */
        JPH::ValidateResult OnContactValidate(
          const JPH::Body& in_body1,
          const JPH::Body& in_body2,
          JPH::RVec3Arg in_base_offset,
          const JPH::CollideShapeResult& in_collision_result) override;

        /**
         * @brief This gets triggered each time a collision comes into contact.
         * It is only called once and then removed.
         *
         * @param body1 Body that called
         * @param body2 Target body
         * @param manifold This class has a lot of stuff including heights,
         * directions, offset etc...
         * @param settings This has some of the body settings these objects
         * have.
         */
        void OnContactAdded(const JPH::Body& body1,
                            const JPH::Body& body2,
                            const JPH::ContactManifold& manifold,
                            JPH::ContactSettings& settings) override;

        /**
         * @brief This gets called multiple times. It is not in use yet.
         *
         * @param in_body1 Body that called
         * @param in_body2 Target body
         * @param in_manifold This class has a lot of stuff including heights,
         * directions, offset etc...
         * @param io_settings This specifically descibes the settings that
         * contacts should have.
         * FIXME: Still needs to be implemented
         */
        void OnContactPersisted(const JPH::Body& in_body1,
                                const JPH::Body& in_body2,
                                const JPH::ContactManifold& in_manifold,
                                JPH::ContactSettings& io_settings) override;
        /**
         * @brief This is used to clean to shapes and call exiting functions for
         * contact.
         *
         * @param in_sub_shape_pair The pair of shapes that no longer touch.
         * FIXME: Still needs to be implemented
         */
        void OnContactRemoved(
          const JPH::SubShapeIDPair& in_sub_shape_pair) override;

    public:
        /**
         * @brief Construct a new contact listener object
         *
         */
        contact_listener();

        /**
         * @brief Deletes all events. Helps reset the collisions on load, etc...
         *
         */
        void clear_events();

        /**
         * @brief Calls all events that came into contact this frame
         *
         */
        void run_events_added();

        /**
         * @brief Runs all events that came into contact every frame but first
         * and last.
         *
         */
        void run_events_persisted();

        /**
         * @brief Runs all events who stopped contact this frame
         *
         */
        void run_events_removed();

    private:
        /**
         * @brief A way to gather all the events and organize them each frame
         *
         */
        std::vector<contact_event> m_contacts_added;

        /**
         * @brief FIXME: These two are unused and will be used when
         * OnContactPersisted and OncontactRemoved get implemented.
         *
         */
        std::vector<contact_event> m_contacts_persisted;
        std::vector<contact_event> m_contacts_removed;

        /**
         * @brief Access to this scene. Should be replaced with get_scenes()
         * function. So it can update based on loading and scene changes.
         *
         */
        ref<scene_scope> m_scene;
        flecs::world m_registry;
    };
};