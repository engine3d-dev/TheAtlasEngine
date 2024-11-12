#include "Physics/Interfaces/BPLayerInterfaceHandler.hpp"
#include <Scenes/Assets/Components/Bodies/Shapes/SphereShaper.hpp>
using namespace JPH;
using namespace JPH::literals;
using namespace engine3d;
SphereShaper::SphereShaper()
{
    JoltHandler * temp = engine3d::JoltHandler::GetInstance();
    m_BodySettings = BodyCreationSettings(
        temp->m_SphereShape,
        RVec3(0.0_r, 4.0_r, 0.0_r),
        Quat::sIdentity(),
        EMotionType::Dynamic,
        Engine3DLayers::Dynamic
        );
        
    m_BodyID = temp->getInterface()->CreateAndAddBody(
                    m_BodySettings, 
                    EActivation::Activate);
}