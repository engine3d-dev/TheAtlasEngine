#include <Physics/Interfaces/BPLayerInterfaceHandler.hpp>
#include <Jolt/Physics/Collision/ObjectLayer.h>
namespace engine3d
{
class ObjectLayerPairFilterInterface : public JPH::ObjectLayerPairFilter
{
    public:
        //! @note TODO: After showcase we should fix this, because doesnt rely on child sub-classes.
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