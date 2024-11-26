#include <Scenes/Assets/Components/Bodies/BodyContainer.hpp>
using namespace JPH::literals;

JPH::BodyID BodyContainer::CreateBody(EActivation p_ActivationType,JPH::RVec3 p_Position)
    {
        engine3d::JoltHandler* l_Handler = engine3d::JoltHandler::GetInstance();
        auto* l_Shape = &ParentShape;

        if(m_BodySettings == nullptr)
        {
            // Create the type of body framework
            m_BodySettings = new BodyCreationSettings(
            *l_Shape,
            p_Position,
            Quat::sIdentity(),
            m_MotionType,
            m_LayerType
            );
        }
        else
        {
            m_BodySettings->SetShape(*l_Shape);
            l_Handler->getInterface()->RemoveBody(m_BodyID);
            l_Handler->getInterface()->DestroyBody(m_BodyID);
            m_BodyID = l_Handler->getInterface()->CreateAndAddBody(*m_BodySettings, p_ActivationType);
            return m_BodyID; 
        }

        
        printf("Getting here5.3\n");

        return m_BodyID = l_Handler->getInterface()->CreateAndAddBody(
            *m_BodySettings,p_ActivationType);
    }

void BodyContainer::CreateRotatedType(JPH::Vec3 p_Rot)
{
    ParentShape =  new RotatedTranslatedShape(
        BaseShape->GetCenterOfMass(),                              
        Quat::sEulerAngles(p_Rot),
        BaseShape                                           
    );
}
void BodyContainer::CreateScaledType(JPH::Vec3 p_Scale)
{
    ParentShape = new ScaledShape(
        BaseShape,
        p_Scale
    );
}
void BodyContainer::CreateCompoundType(JPH::Vec3 p_Rot, JPH::Vec3 p_Scale)
{
    //In this order to prevent sheering
    // Scale
    ParentShape = new ScaledShape(
        BaseShape,
        p_Scale
    );

    // Rotate
    ParentShape =  new RotatedTranslatedShape(
        ParentShape->GetCenterOfMass(),                              
        Quat::sEulerAngles(p_Rot),
        ParentShape                                           
    );


}

BodyContainer::~BodyContainer()
{
    delete ParentShape;
    delete BaseShape;
}