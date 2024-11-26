#pragma once
#include <Scenes/Assets/Components/Physics/PhysicsBody3D.hpp>
class BoxShaper : public BodyContainer
{
    public:
        BoxShaper() = delete;
        BoxShaper(JPH::EMotionType p_MotionType, const JPH::ObjectLayer p_LayerType);
};