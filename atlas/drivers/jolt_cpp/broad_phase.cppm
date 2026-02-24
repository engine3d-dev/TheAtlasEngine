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

export module atlas.drivers.jolt_cpp.broad_phase;

import atlas.common;

export namespace atlas::physics {

    /**
     * @brief This contains a few important comparisons having to do with
     * setting up the oct-trees correctly as well as the filtering and collision
     * interaction.
     *
     * @warning Most of these contain implicit conversions which might lead to
     * unexpected behavior. This should probably be fixed or atleast checked
     * eventually.
     */

    // This should eventually have a pipeline for the user to create masks.
    enum class object_layer : std::uint8_t {
        non_moving = 0,
        moving = 1,
        num_layers
    };

    enum class broad_phase_layer : std::uint8_t {
        non_moving = 0,
        moving = 1,
        num_layers
    };

    /**
     * @brief This class is made to control the broadphase layer. Filters can be
     * added to it to create a better and more organized broadphase. Ass well as
     * giving some control to the user on how the broadphase should be
     * organized. This requires dynamic masking however, which is not currently
     * running.
     *
     */
    class broad_phase_layer_interface final
      : public JPH::BroadPhaseLayerInterface {
    public:
        broad_phase_layer_interface() = default;

        /**
         * @brief Get the Number of layers that exsist in the current context.
         * Static for now.
         *
         * @return uint32_t
         */
        [[nodiscard]] uint32_t GetNumBroadPhaseLayers() const override {
            return (uint32_t)(broad_phase_layer::num_layers);
        }

        /**
         * @brief Gives the caller access to the broadphase object. Allowing for
         * some manipulation on how those interactions may go. Including but not
         * limited to changing the filtering of the broadphase or reorganizing
         * the oct-tree.
         *
         * @param p_in_layer The layer that the player wants.
         * @return JPH::BroadPhaseLayer Object to organize and manipulate the
         * oct-tree for that layer.
         */
        [[nodiscard]] JPH::BroadPhaseLayer GetBroadPhaseLayer(
          JPH::ObjectLayer p_in_layer) const override {
            JPH_ASSERT(p_in_layer <
                       static_cast<JPH::ObjectLayer>(object_layer::num_layers));
            return m_object_to_broadphase[p_in_layer];
        }

#if defined(JPH_EXTERNAL_PROFILE) || defined(JPH_PROFILE_ENABLED)
        /**
         * @brief Only exsists for testing contexts or debug mode. Is made to
         * return the names of the layers. Will be very helpful when the layers
         * become dynamically created.
         *
         * @param p_in_layer The layer that the player wants as a name.
         * @return const char* Name of the layer.
         */
        [[nodiscard]] const char* GetBroadPhaseLayerName(
          JPH::BroadPhaseLayer p_in_layer) const override {
            switch (p_in_layer.GetValue()) {
                case static_cast<uint8_t>(broad_phase_layer::non_moving):
                    return "NonMoving";
                case static_cast<uint8_t>(broad_phase_layer::moving):
                    return "Moving";
                default:
                    JPH_ASSERT(false);
                    return "Unknown";
            }
        }
#endif

    private:
        // The list of organizational layers
        std::vector<JPH::BroadPhaseLayer> m_object_to_broadphase{
            JPH::BroadPhaseLayer(
              static_cast<uint8_t>(broad_phase_layer::non_moving)),
            JPH::BroadPhaseLayer(
              static_cast<uint8_t>(broad_phase_layer::moving))

        };
    };

    /**
     * @brief This is used to tell Jolt what can or cannot collide. As of right
     * now the list is static therfore the layers do not need a dynamic set up.
     * This will change when the object layers become user definable.
     *
     * @remark To give an example, there might exist something like this in the
     * config of the editor:
     *
     *          Dynamic Kenmatic  Static
     * Dynamic  ✅      ✅        ✅
     * Kenmatic ✅      ✅
     * Static   ✅
     *
     * Where the interactions can partially be defined and controlled by the
     * developer of the game.
     */
    class object_vs_broadphase_layer final
      : public JPH::ObjectVsBroadPhaseLayerFilter {
    public:
        [[nodiscard]] bool ShouldCollide(
          JPH::ObjectLayer p_in_layer1,
          JPH::BroadPhaseLayer p_in_layer2) const override {
            switch (p_in_layer1) {
                case (int)(object_layer::non_moving):
                    return p_in_layer2 ==
                           JPH::BroadPhaseLayer(
                             static_cast<uint8_t>(broad_phase_layer::moving));
                case (int)object_layer::moving:
                    return true;
                default:
                    JPH_ASSERT(false);
                    return false;
            }
        }
    };

    /**
     * @brief This goes into more detailed ways of filtering, where the object
     * collisions may be defined be what the other object is. In this case a
     * static setup allows static object to trigger collision only when it is
     * touched by a dynamic target.
     *
     */
    class object_layer_pair_filter final : public JPH::ObjectLayerPairFilter {
    public:
        [[nodiscard]] bool ShouldCollide(
          JPH::ObjectLayer p_in_object1,
          JPH::ObjectLayer p_in_object2) const override {
            switch (p_in_object1) {
                case static_cast<int>(object_layer::non_moving):
                    return p_in_object2 ==
                           static_cast<int>(object_layer::moving);
                case static_cast<int>(object_layer::moving):
                    return true;
                default:
                    JPH_ASSERT(false);
                    return false;
            }
        }
    };

}