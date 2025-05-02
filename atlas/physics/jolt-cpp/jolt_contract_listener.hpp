#include <Jolt/Jolt.h>
#include <Jolt/Physics/Collision/ContactListener.h>

class contact_listener : public JPH::ContactListener
{
public:
	// See: ContactListener
	JPH::ValidateResult	OnContactValidate(const JPH::Body &in_body1, const JPH::Body &in_body2, JPH::RVec3Arg in_base_offset, const JPH::CollideShapeResult &in_collision_result) override;

	void OnContactAdded(const JPH::Body &in_body1, const JPH::Body &in_body2, const JPH::ContactManifold &in_manifold, JPH::ContactSettings &io_settings) override;
	void OnContactPersisted(const JPH::Body &in_body1, const JPH::Body &in_body2, const JPH::ContactManifold &in_manifold, JPH::ContactSettings &io_settings) override;

	void OnContactRemoved(const JPH::SubShapeIDPair &in_sub_shape_pair) override;
};