#include <Jolt/Jolt.h>
#include <Jolt/Physics/Collision/ContactListener.h>
#include <physics/physics_3d/data/contact_event.hpp>
#include <vector>

namespace atlas::physics {
    class contact_listener : public JPH::ContactListener {
    public:

        contact_listener();
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

        void clear_events();

    private:
        //! @note This is a temparary solve, waiting for event handlers
        std::vector<contact_event> m_contact_list;
        flecs::world m_registry;
    };
};