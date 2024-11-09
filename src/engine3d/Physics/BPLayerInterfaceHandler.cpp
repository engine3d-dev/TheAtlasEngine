#include <Jolt/Core/Core.h>
#include <Physics/BPLayerInterfaceHandler.hpp>

#include <Jolt/Physics/Collision/BroadPhase/BroadPhaseLayer.h>
#include <Jolt/Physics/Collision/ObjectLayer.h>


namespace Engine3DLayers
{
    static constexpr JPH::ObjectLayer Static = 0;
	static constexpr JPH::ObjectLayer Kenmatic = 1;
    static constexpr JPH::ObjectLayer Dynamic = 2;
	static constexpr JPH::ObjectLayer NUM_LAYERS = 3;
};

namespace Engine3DBroadPhaseLayers
{
	static constexpr JPH::BroadPhaseLayer NON_MOVING(0);
	static constexpr JPH::BroadPhaseLayer MOVING(1);
	static constexpr JPH::uint NUM_LAYERS(2);
};


namespace engine3d
{
    BPLayerInterfaceHandler::BPLayerInterfaceHandler()
    {
        
    }
};