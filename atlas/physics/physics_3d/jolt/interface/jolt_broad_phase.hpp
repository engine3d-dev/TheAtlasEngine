#pragma once

#include <physics/jolt-cpp/jolt-imports.hpp>

namespace atlas::physics {

    enum class ObjectLayer : std::uint8_t {
        NonMoving = 0,
        Moving = 1,
        NumLayers
    };

    enum class BroadPhaseLayers : std::uint8_t {
        NonMoving = 0,
        Moving = 1,
        NumLayers
    };

    class broad_phase_layer_interface final
      : public JPH::BroadPhaseLayerInterface {
    public:
        broad_phase_layer_interface() = default;

        [[nodiscard]] uint32_t GetNumBroadPhaseLayers() const override {
            return (uint32_t)(BroadPhaseLayers::NumLayers);
        }

        [[nodiscard]] JPH::BroadPhaseLayer GetBroadPhaseLayer(
          JPH::ObjectLayer p_in_layer) const override {
            JPH_ASSERT(p_in_layer < ObjectLayer::NumLayers);
            return m_object_to_broadphase[p_in_layer];
        }
        
#if defined(JPH_EXTERNAL_PROFILE) || defined(JPH_PROFILE_ENABLED)
        [[nodiscard]] const char* GetBroadPhaseLayerName(
          JPH::BroadPhaseLayer p_in_layer) const override {
            switch (p_in_layer.GetValue()) {
                case (JPH::BroadPhaseLayer::Type)(
                  BroadPhaseLayers::NonMoving):
                    return "NonMoving";
                case (JPH::BroadPhaseLayer::Type)(
                  BroadPhaseLayers::Moving):
                    return "Moving";
                default:
                    JPH_ASSERT(false);
                    return "Unknown";
            }
        }
#endif

    private:
        std::vector<JPH::BroadPhaseLayer> m_object_to_broadphase{
            JPH::BroadPhaseLayer(
              (uint8_t)(BroadPhaseLayers::NonMoving)),
            JPH::BroadPhaseLayer((uint8_t)(BroadPhaseLayers::Moving))

        };
    };

    class object_vs_broadphase_layer final
      : public JPH::ObjectVsBroadPhaseLayerFilter {
    public:
        [[nodiscard]] bool ShouldCollide(
          JPH::ObjectLayer p_in_layer1,
          JPH::BroadPhaseLayer p_in_layer2) const override {
            switch (p_in_layer1) {
                case (int)(ObjectLayer::NonMoving):
                    return p_in_layer2 ==
                           JPH::BroadPhaseLayer(
                             (JPH::BroadPhaseLayer::Type)(
                               BroadPhaseLayers::Moving));
                case (int)ObjectLayer::Moving:
                    return true;
                default:
                    JPH_ASSERT(false);
                    return false;
            }
        }
    };

    class object_layer_pair_filter final : public JPH::ObjectLayerPairFilter {
    public:
        [[nodiscard]] bool ShouldCollide(
          JPH::ObjectLayer p_in_object1,
          JPH::ObjectLayer p_in_object2) const override {
            switch (p_in_object1) {
                case (int)(ObjectLayer::NonMoving):
                    return p_in_object2 ==
                           (int)(ObjectLayer::Moving);
                case (int)(ObjectLayer::Moving):
                    return true;
                default:
                    JPH_ASSERT(false);
                    return false;
            }
        }
    };

}
