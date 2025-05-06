#include <Jolt/Jolt.h>
#include <Jolt/Physics/Collision/ContactListener.h>
#include <physics/physics_3d/data/contact_event.hpp>
#include <vector>

namespace atlas::physics {
    class contact_listener : public JPH::ContactListener {
    public:
        JPH::ValidateResult OnContactValidate(
          const JPH::Body& in_body1,
          const JPH::Body& in_body2,
          JPH::RVec3Arg in_base_offset,
          const JPH::CollideShapeResult& in_collision_result) override;

        void OnContactAdded(const JPH::Body& in_body1,
                            const JPH::Body& in_body2,
                            const JPH::ContactManifold& in_manifold,
                            JPH::ContactSettings& io_settings) override;
        void OnContactPersisted(const JPH::Body& in_body1,
                                const JPH::Body& in_body2,
                                const JPH::ContactManifold& in_manifold,
                                JPH::ContactSettings& io_settings) override;

        void OnContactRemoved(
          const JPH::SubShapeIDPair& in_sub_shape_pair) override;

		void run_all_contacts();

		private:
			std::vector<contact_event> m_contact_list;
    };
};