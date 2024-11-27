#pragma once
#include <Scenes/Assets/Components/Physics/PhysicsBody3D.hpp>
class SphereShaper : public BodyContainer
{
    public:
        SphereShaper() = delete;
        SphereShaper(JPH::EMotionType p_MotionType, const JPH::ObjectLayer p_LayerType);
};