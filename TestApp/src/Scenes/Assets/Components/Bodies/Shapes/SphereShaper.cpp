#include <Scenes/Assets/Components/Bodies/Shapes/SphereShaper.hpp>
#include <Jolt/Physics/Collision/Shape/SphereShape.h>
SphereShaper::SphereShaper(JPH::EMotionType p_MotionType, const JPH::ObjectLayer p_LayerType)
{
    BaseShape = new SphereShape(2.5f);
    ParentShape = BaseShape;
    m_MotionType = p_MotionType;
    m_LayerType = p_LayerType;
}