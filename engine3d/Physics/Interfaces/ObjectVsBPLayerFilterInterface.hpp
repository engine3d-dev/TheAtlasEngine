
#include <Physics/Interfaces/BPLayerInterfaceHandler.hpp>
#include <Jolt/Physics/Collision/BroadPhase/BroadPhaseLayer.h>
#include <Jolt/Physics/Collision/ObjectLayer.h>

namespace engine3d {
	class ObjectVsBPLayerFilterInterface : public JPH::ObjectVsBroadPhaseLayerFilter
	{
		public:
		virtual bool ShouldCollide(JPH::ObjectLayer inLayer1, JPH::BroadPhaseLayer inLayer2) const override
		{
			switch (inLayer1)
			{
			case Engine3DLayers::Static:
				return (inLayer2 == Engine3DBroadPhaseLayers::Dynamic ||
				 		inLayer2 == Engine3DBroadPhaseLayers::Kenmatic);
			case Engine3DLayers::Kenmatic:
				return true;
			case Engine3DLayers::Dynamic:
				return true;
			default:
				JPH_ASSERT(false);
				return false;
			}
		}
	};
};