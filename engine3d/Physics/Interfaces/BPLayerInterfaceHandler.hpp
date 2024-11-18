#pragma once
#include <Jolt/Jolt.h>
#include <Jolt/Core/Core.h>
#include <Jolt/Physics/Collision/BroadPhase/BroadPhaseLayer.h>
#include <Jolt/Physics/Collision/ObjectLayer.h>
#include <Jolt/Core/IssueReporting.h>

namespace Engine3DLayers
{
    static constexpr JPH::ObjectLayer Static = 0;
	static constexpr JPH::ObjectLayer Kenmatic = 1;
    static constexpr JPH::ObjectLayer Dynamic = 2;
	static constexpr JPH::ObjectLayer NUM_LAYERS = 3;
};

namespace Engine3DBroadPhaseLayers
{
	static constexpr JPH::BroadPhaseLayer Static(0);
	static constexpr JPH::BroadPhaseLayer Kenmatic(1);
    static constexpr JPH::BroadPhaseLayer Dynamic(2);
	static constexpr JPH::uint NUM_LAYERS(3);
};

namespace engine3d
{
    class BPLayerInterfaceHandler : public JPH::BroadPhaseLayerInterface
    {
        public:
            BPLayerInterfaceHandler()
            {
                m_SceneObjToBroadPhase[Engine3DLayers::Static] = Engine3DBroadPhaseLayers::Static;
                m_SceneObjToBroadPhase[Engine3DLayers::Kenmatic] = Engine3DBroadPhaseLayers::Kenmatic;
                m_SceneObjToBroadPhase[Engine3DLayers::Dynamic] = Engine3DBroadPhaseLayers::Dynamic;
            }

            virtual JPH::uint GetNumBroadPhaseLayers() const override
            {
                return Engine3DBroadPhaseLayers::NUM_LAYERS;
            }
            
            virtual JPH::BroadPhaseLayer GetBroadPhaseLayer(JPH::ObjectLayer inLayer) const override
            {
                JPH_ASSERT(inLayer < Engine3DLayers::NUM_LAYERS);
                return m_SceneObjToBroadPhase[inLayer];
            }

            //Debug stuff for profiling
            #if defined(JPH_EXTERNAL_PROFILE) || defined(JPH_PROFILE_ENABLED)
                virtual const char *			GetBroadPhaseLayerName(JPH::BroadPhaseLayer inLayer) const override
                {
                    switch ((JPH::BroadPhaseLayer::Type)inLayer)
                    {
                    case (JPH::BroadPhaseLayer::Type)Engine3DBroadPhaseLayers::Static:	return "Static";
                    case (JPH::BroadPhaseLayer::Type)Engine3DBroadPhaseLayers::Kenmatic:  return "Kenmatic";
                    case (JPH::BroadPhaseLayer::Type)Engine3DBroadPhaseLayers::Dynamic:   return "Dynamic";
                    default:			                                            JPH_ASSERT(false); return "INVALID";
                    }
                }
            #endif

        private:
            //Places an object into a substruct known as broadphase
            JPH::BroadPhaseLayer m_SceneObjToBroadPhase[Engine3DLayers::NUM_LAYERS];
    };
};