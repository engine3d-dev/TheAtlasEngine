#include <Jolt/Jolt.h>
#include <Jolt/Physics/Collision/ContactListener.h>


#include <print>

using namespace JPH;

namespace engine3d
{
    class Engine3DContactlistener : public ContactListener
    {
        virtual ValidateResult	OnContactValidate(const Body &inBody1, const Body &inBody2, RVec3Arg inBaseOffset, const CollideShapeResult &inCollisionResult) override
        {
            std::print("Validating...\n");

            // Allows you to ignore a contact before it is created (using layers to not make objects collide is cheaper!)
            return ValidateResult::AcceptAllContactsForThisBodyPair;
        }

        virtual void OnContactAdded(const Body &inBody1, const Body &inBody2, const ContactManifold &inManifold, ContactSettings &ioSettings) override
        {
            std::print("Contact was added!\n");
        }

        virtual void OnContactPersisted(const Body &inBody1, const Body &inBody2, const ContactManifold &inManifold, ContactSettings &ioSettings) override
        {
            std::print("Contact was persisted!\n");
        }

        virtual void OnContactRemoved(const SubShapeIDPair &inSubShapePair) override
        {
            std::print("Contact was removed!\n");
        }
    };
};