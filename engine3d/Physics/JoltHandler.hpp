#pragma once
// Engine3d includes
#include <Physics/Interfaces/BPLayerInterfaceHandler.hpp>
#include <Physics/Interfaces/ObjectVsBPLayerFilterInterface.hpp>
#include <Physics/Interfaces/ObjectLayerPairFilterInterface.hpp>
#include <Physics/Interaction/Engine3DActivationListener.hpp>
#include <Physics/Interaction/Engine3DContactListener.hpp>

// Jolt includes
#include <Jolt/Physics/Collision/Shape/ConvexShape.h>
#include <Jolt/Physics/Collision/Shape/ScaledShape.h>
#include <Jolt/Physics/Collision/Shape/SphereShape.h>
#include <Jolt/Physics/Body/BodyCreationSettings.h>
#include <Jolt/Physics/Collision/Shape/BoxShape.h>
#include <Jolt/Physics/Body/BodyInterface.h>
#include <Jolt/Core/JobSystemThreadPool.h>
#include <Jolt/Physics/PhysicsSettings.h>
#include <Jolt/Physics/PhysicsSystem.h>
#include <Jolt/Core/IssueReporting.h>
#include <Jolt/Core/TempAllocator.h>
#include <Jolt/Core/TempAllocator.h>
#include <Jolt/RegisterTypes.h>
#include <Jolt/Core/Factory.h>
#include <Jolt/Core/Memory.h>
namespace engine3d
{
    class JoltHandler
    {
        public:
            JoltHandler();
            ~JoltHandler();

            static JoltHandler* GetInstance() 
            {
                static JoltHandler instance;
                return &instance;
            }

            BodyInterface* getInterface();

            // Actuall physics part
            JPH::PhysicsSystem physics_system;

            TempAllocatorImpl* physics_allocator;

            JPH::JobSystemThreadPool* job_system;

            // Substructs for seperating types of bodies interface
            BPLayerInterfaceHandler broad_phase_layer_interface;

            // Filter for body types filter using broadphase
            ObjectVsBPLayerFilterInterface object_vs_broadphase_layer_filter;

            // Filter for body types using Object Layers
            ObjectLayerPairFilterInterface object_vs_object_layer_filter;

            // Setting Contact listener
            Engine3DContactlistener contact_listener;

            // Setting Activation listener
            Engine3DActivationListener body_activation_listener;

            // interface for the bodies
            BodyInterface* body_interface;
    };
};