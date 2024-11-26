#include <Scenes/Assets/Components/Bodies/Shapes/BoxShaper.hpp>
#include <Jolt/Physics/Collision/Shape/BoxShape.h>
BoxShaper::BoxShaper(JPH::EMotionType p_MotionType, const JPH::ObjectLayer p_LayerType)
{
    printf("Getting here2\n");
    BaseShape = new JPH::BoxShape(Vec3(1.0f, 1.0f, 1.0f));
    ParentShape = BaseShape;
    printf("Getting here3\n");
    m_MotionType = p_MotionType;
    m_LayerType = p_LayerType;
}