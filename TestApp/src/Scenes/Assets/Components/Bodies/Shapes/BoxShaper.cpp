#include <Scenes/Assets/Components/Bodies/Shapes/BoxShaper.hpp>
using namespace JPH;
using namespace JPH::literals;
using namespace engine3d;
BoxShaper::BoxShaper()
{
    JoltHandler * temp = engine3d::JoltHandler::GetInstance();
    m_BodySettings = BodyCreationSettings(
        temp->m_BoxShapeScaled,
        RVec3(0.0_r, -1.0_r, 0.0_r),
        Quat::sIdentity(),
        EMotionType::Static,
        Engine3DLayers::Static
        );
        
    m_BodyID = temp->getInterface()->CreateAndAddBody(
                    m_BodySettings, 
                    EActivation::DontActivate);
}