#include <Jolt/Jolt.h>
#include <Jolt/Physics/Body/BodyActivationListener.h>

#include <print>

using namespace JPH;

namespace engine3d
{
    class Engine3DActivationListener : public JPH::BodyActivationListener
    {
        public:
            virtual void OnBodyActivated(const BodyID &inBodyID, uint64 inBodyUserData) override
            {
                std::print("Body was activated!\n");
            }

            virtual void OnBodyDeactivated(const BodyID &inBodyID, uint64 inBodyUserData) override
            {
                std::print("Body is asleep...\n");
            }
    };
};