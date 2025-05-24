#pragma once

#include <Jolt/Jolt.h>
#include <Jolt/Physics/Collision/ContactListener.h>
#include <physics/jolt-cpp/jolt_components.hpp>
#include <core/scene/scene.hpp>

namespace atlas::physics {

    class contact_listener : public JPH::ContactListener {
    private:
        JPH::ValidateResult OnContactValidate(
          const JPH::Body& in_body1,
          const JPH::Body& in_body2,
          JPH::RVec3Arg in_base_offset,
          const JPH::CollideShapeResult& in_collision_result) override;

        void OnContactAdded(const JPH::Body& body1,
                            const JPH::Body& body2,
                            const JPH::ContactManifold& manifold,
                            JPH::ContactSettings& settings) override;

        void OnContactPersisted(const JPH::Body& in_body1,
                                const JPH::Body& in_body2,
                                const JPH::ContactManifold& in_manifold,
                                JPH::ContactSettings& io_settings) override;

        void OnContactRemoved(
          const JPH::SubShapeIDPair& in_sub_shape_pair) override;

    public:
        contact_listener();
        void clear_events();
        void run_events_added();
        void run_events_persisted();
        void run_events_removed();

    private:
        //! @note This is a temparary solve, waiting for event handlers
        std::vector<jolt::contact_event> m_contacts_added;
        std::vector<jolt::contact_event> m_contacts_persisted;
        std::vector<jolt::contact_event> m_contacts_removed;

        ref<scene_scope> m_scene;
        flecs::world m_registry;
    };
};