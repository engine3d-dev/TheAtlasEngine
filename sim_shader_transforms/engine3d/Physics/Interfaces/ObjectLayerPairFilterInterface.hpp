#include <Physics/Interfaces/BPLayerInterfaceHandler.hpp>
#include <Jolt/Physics/Collision/ObjectLayer.h>
namespace engine3d
{
class ObjectLayerPairFilterInterface : public JPH::ObjectLayerPairFilter
{
    public:
        virtual bool ShouldCollide(JPH::ObjectLayer inObject1, JPH::ObjectLayer inObject2) const override
        {
            switch (inObject1)
            {
            case Engine3DLayers::Static:
                return (inObject2 == Engine3DLayers::Dynamic ||
                        inObject2 == Engine3DLayers::Kenmatic);
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