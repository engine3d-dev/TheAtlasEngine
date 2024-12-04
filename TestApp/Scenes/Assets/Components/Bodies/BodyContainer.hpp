#pragma once
// #include <engine3d/Physics/JoltHandler.hpp>
#include "Physics/Interfaces/BPLayerInterfaceHandler.hpp"
#include <Jolt/Core/Core.h>
#include <Jolt/Physics/Collision/Shape/Shape.h>
#include <engine3d/Physics/JoltHandler.hpp>
// Jolt Includes
#include <Jolt/Physics/Body/MotionType.h>
#include <Jolt/Physics/Collision/ObjectLayer.h>
#include <Jolt/Physics/Collision/Shape/RotatedTranslatedShape.h>
#include <Jolt/Physics/Collision/Shape/ScaledShape.h>
#include <Jolt/Physics/Collision/Shape/CompoundShape.h>
//Other includes
#include <glm/fwd.hpp>
class BodyContainer
{
    public:
        BodyContainer() = default;
        ~BodyContainer();

        operator JPH::BodyID() { return m_BodyID; }
        JPH::BodyCreationSettings* m_BodySettings = nullptr;
        JPH::BodyID m_BodyID;
        
        void CreateRotatedType(JPH::Vec3 p_Rot);
        void CreateScaledType(JPH::Vec3 p_Scale);
        void CreateCompoundType(JPH::Vec3 p_Rot, JPH::Vec3 p_Scale);
        JPH::BodyID CreateBody(EActivation p_ActivationType ,JPH::RVec3 p_Position);

    protected:
        Ref<Shape> BaseShape = nullptr;
        Ref<Shape> ParentShape = nullptr;
        JPH::EMotionType m_MotionType = EMotionType::Static;
        JPH::ObjectLayer m_LayerType = Engine3DLayers::Static;
    

};