module;

#include <Jolt/Jolt.h>
#include <Jolt/Core/Core.h>
#include <Jolt/Physics/Body/BodyInterface.h>
#include <Jolt/Core/JobSystemThreadPool.h>
#include <Jolt/Physics/PhysicsSettings.h>
#include <Jolt/Core/IssueReporting.h>
#include <Jolt/Core/TempAllocator.h>
#include <Jolt/RegisterTypes.h>
#include <Jolt/Core/Factory.h>
#include <Jolt/Core/Memory.h>
#include <Jolt/Physics/Collision/ContactListener.h>
#include <Jolt/Physics/PhysicsSystem.h>
#include <Jolt/Physics/Collision/ObjectLayer.h>
#include <Jolt/Physics/Collision/BroadPhase/BroadPhaseLayer.h>
#include <Jolt/Core/IssueReporting.h>

#include <Jolt/Physics/Body/BodyCreationSettings.h>

#include <Jolt/Physics/Collision/Shape/Shape.h>
#include <Jolt/Physics/Collision/Shape/BoxShape.h>
#include <Jolt/Physics/Collision/Shape/ConvexShape.h>
#include <Jolt/Physics/Collision/Shape/CapsuleShape.h>
#include <Jolt/Physics/Collision/Shape/ScaledShape.h>
#include <Jolt/Physics/Collision/Shape/SphereShape.h>

#include <Jolt/Physics/EActivation.h>

// jolt's math includes
#include <Jolt/Math/MathTypes.h>
#include <Jolt/Math/Real.h>
#include <Jolt/Math/Quat.h>

export module atlas.drivers.jolt_cpp.contact_listener;
import atlas.logger;
import atlas.core.event.bus;
import atlas.core.event.types;

export namespace atlas::physics {
    /**
     * @brief implementation of Jolt's contact listener for collisions
     *
     * contact_listener gets set to Jolt's Physics System to allow for
     * collisions to happen
     */
    class contact_listener : public JPH::ContactListener {
    public:
        contact_listener(event::bus& p_bus) : m_bus(&p_bus) {}

    private:
        /**
         * @brief This allows us to make sure that the contacts are valid and
         * not something that went wrong. It plays the role of both sanity check
         * and saftey gaurd since if collisions were to be wrong you would not
         * want that to crash the whole game.
         *
         * @param in_body1 is ID of the object1 who collided
         * @param in_body2 is ID of object2 who object1 collided with
         * @param in_base_offset for telling how far apart the center of the
         * objects are
         * @param in_collision_result The details about the collision
         * @return JPH::ValidateResult
         */
        JPH::ValidateResult OnContactValidate(
          const JPH::Body& in_body1,
          const JPH::Body& in_body2,
          JPH::RVec3Arg in_base_offset,
          const JPH::CollideShapeResult& in_collision_result) override {
            return JPH::ValidateResult::AcceptAllContactsForThisBodyPair;
        }

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
        void OnContactAdded(const JPH::Body& p_body1,
                            const JPH::Body& p_body2,
                            const JPH::ContactManifold& manifold,
                            JPH::ContactSettings& settings) override {
             event::collision_enter begin_event = {
                .entity1 = static_cast<uint64_t>(p_body1.GetUserData()),
                .entity2 = static_cast<uint64_t>(p_body2.GetUserData())
            };

            // Publishes to all subscribers that this collision_enter event has
            // occurred
            m_bus->publish(begin_event);
        }

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
        void OnContactPersisted(const JPH::Body& p_body1,
                                const JPH::Body& p_body2,
                                const JPH::ContactManifold& in_manifold,
                                JPH::ContactSettings& io_settings) override {
            event::collision_persisted persisted_event = {
                .entity1 = static_cast<uint64_t>(p_body1.GetUserData()),
                .entity2 = static_cast<uint64_t>(p_body2.GetUserData())
            };

            m_bus->publish(persisted_event);
        }

        /**
         * @brief This is used to clean to shapes and call exiting functions for
         * contact.
         *
         * @param in_sub_shape_pair The pair of shapes that no longer touch.
         * FIXME: Still needs to be implemented
         */
        void OnContactRemoved(const JPH::SubShapeIDPair& p_sub_shape_pair) override {
            console_log_info("Collisions Removed!");
            // For Event system to handle when collision ends
        }

    private:
        event::bus* m_bus;
    };
};