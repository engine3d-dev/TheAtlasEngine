#pragma once
#include <physics/jolt-cpp/jolt-imports.hpp>
#include <Jolt/Physics/Collision/ObjectLayer.h>
#include <Jolt/Physics/Body/BodyManager.h>
#include <Jolt/Physics/Body/BodyActivationListener.h>
#include <Jolt/Physics/Collision/ContactListener.h>
#include <array>

namespace atlas::physics{

    namespace phase_layer{
        static constexpr JPH::BroadPhaseLayer STATIC(0);
    };

    //! @note Physics settings per physic scene
    struct setting{
        float gravity = -10.f;
    };


    // enum class BodyTypeTest : int8_t {
    //     UNDEFINED = -1,
    //     STATIC = 0,
    //     KINEMATIC = 1,
    //     DYNAMIC = 2
    // };

    namespace BodyTypeTest{
        static constexpr JPH::ObjectLayer STATIC = 0;
        static constexpr JPH::ObjectLayer KINEMATIC = 1;
        static constexpr JPH::ObjectLayer DYNAMIC = 2;
    };

    namespace  BroadPhaseLayers{
        static constexpr JPH::BroadPhaseLayer STATIC(0);
        static constexpr JPH::BroadPhaseLayer KINEMATIC(1);
        static constexpr JPH::BroadPhaseLayer DYNAMIC(2);
    };

    class BPLayerInterfaceHandler : public JPH::BroadPhaseLayerInterface{
    public:
        BPLayerInterfaceHandler(){
            m_ToBroadPhase[BodyTypeTest::STATIC] = BroadPhaseLayers::STATIC;
            m_ToBroadPhase[BodyTypeTest::DYNAMIC] = BroadPhaseLayers::DYNAMIC;
            m_ToBroadPhase[BodyTypeTest::KINEMATIC] = BroadPhaseLayers::KINEMATIC;
        }

        JPH::uint GetNumBroadPhaseLayers() const override {
            return 3;
        }

        JPH::BroadPhaseLayer GetBroadPhaseLayer(JPH::ObjectLayer inLayer) const override {
            // 3 = num of layers
            JPH_ASSERT(inLayer < 3);
            return m_ToBroadPhase[inLayer];
        }
        #if defined(JPH_EXTERNAL_PROFILE) || defined(JPH_PROFILE_ENABLED)
        const char * GetBroadPhaseLayerName([[maybe_unused]] JPH::BroadPhaseLayer inLayer) const override{
            return "Test";
        }
        #endif

    private:
        std::array<JPH::BroadPhaseLayer, 3> m_ToBroadPhase;
        // JPH::BroadPhaseLayer m_ToBroadPhase[3];
    };

    class ObjectLayerPairFilterInterface : public JPH::ObjectLayerPairFilter{
    public:
        bool ShouldCollide(JPH::ObjectLayer inObject1, JPH::ObjectLayer inObject2) const override{
            switch (inObject1){
            case BodyTypeTest::STATIC:
            {
                return (inObject2 == BodyTypeTest::DYNAMIC ||
                    inObject2 == BodyTypeTest::KINEMATIC);
            }
            case BodyTypeTest::KINEMATIC:
                return true;
            case BodyTypeTest::DYNAMIC:
                return true;
            default:
                return false;
            }
        }
    };

    class ObjectVsBPLayerFilterInterface : public JPH::ObjectVsBroadPhaseLayerFilter{
    public:
        bool ShouldCollide(JPH::ObjectLayer inObject1, JPH::BroadPhaseLayer inObject2) const override{
            switch (inObject1){
            case BodyTypeTest::STATIC:
            {
                return (inObject2 == BroadPhaseLayers::DYNAMIC ||
                    inObject2 == BroadPhaseLayers::KINEMATIC);
            }
            case BodyTypeTest::KINEMATIC:
                return true;
            case BodyTypeTest::DYNAMIC:
                return true;
            default:
                return false;
            }
        }
    };

    class ActivationListener : public JPH::BodyActivationListener{
    public:
        void OnBodyActivated([[maybe_unused]]const JPH::BodyID &inBodyID, [[maybe_unused]] JPH::uint64 inBodyUserData) override{
            // console_log_trace("");
            printf("FROM ActivationListener Class --- Body was activated!\n");
        }

        void OnBodyDeactivated([[maybe_unused]]const JPH::BodyID &inBodyID, [[maybe_unused]] JPH::uint64 inBodyUserData) override{
            // std::print("Body is asleep...\n");
            printf("FROM ActivationListener Class --- Body is asleep...\n");
        }
    };

    class ContactListener : public JPH::ContactListener {
    public:
        JPH::ValidateResult	OnContactValidate([[maybe_unused]] const JPH::Body &inBody1, [[maybe_unused]] const JPH::Body &inBody2, [[maybe_unused]] JPH::RVec3Arg inBaseOffset, [[maybe_unused]] const JPH::CollideShapeResult &inCollisionResult) override{
            printf("FROM ContactListener Class --- Validating...\n");

            // Allows you to ignore a contact before it is created (using layers to not make objects collide is cheaper!)
            return JPH::ValidateResult::AcceptAllContactsForThisBodyPair;
        }

        void OnContactAdded([[maybe_unused]] const JPH::Body &inBody1, [[maybe_unused]] const JPH::Body &inBody2, [[maybe_unused]] const JPH::ContactManifold &inManifold, [[maybe_unused]] JPH::ContactSettings &ioSettings) override{
            printf("FROM ContactListener Class --- Contact was added!\n");
        }

        void OnContactPersisted([[maybe_unused]] const JPH::Body &inBody1, [[maybe_unused]] const JPH::Body &inBody2, [[maybe_unused]] const JPH::ContactManifold &inManifold, [[maybe_unused]] JPH::ContactSettings &ioSettings) override{
            printf("FROM ContactListener Class --- Contact was persisted!\n");
        }

        void OnContactRemoved([[maybe_unused]] const JPH::SubShapeIDPair &inSubShapePair) override{
            printf("FROM ContactListener Class --- Contact was removed!\n");
        }
    };


    //! @note Screw it, I dont have enough time. Just gonna implement physics as I see it currently.
    /**/
    class JoltAPI{
    public:
        //! @note Used for initializing the physic's system API backend
        static void InitializeJoltAPI();





        static JPH::PhysicsSystem& GetSystem();

        static JPH::BodyInterface* GetBodyInterface();
    };
};