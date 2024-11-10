#include <Physics/JoltHandler.hpp>
#include <Physics/Interfaces/BPLayerInterfaceHandler.hpp>
#include <Physics/Interfaces/ObjectVsBPLayerFilterInterface.hpp>
#include <Physics/Interfaces/ObjectLayerPairFilterInterface.hpp>
#include <Physics/Interaction/Engine3DActivationListener.hpp>
#include <Physics/Interaction/Engine3DContactListener.hpp>
//! @note All includes under here except JoltHandler
#include <Jolt/Core/JobSystemThreadPool.h>
#include <Jolt/Physics/PhysicsSettings.h>
#include <Jolt/Physics/PhysicsSystem.h>
#include <Jolt/Core/IssueReporting.h>
#include <Jolt/Core/TempAllocator.h>
#include <Jolt/RegisterTypes.h>
#include <Jolt/Core/Factory.h>
#include <Jolt/Core/Memory.h>
#include <cassert>
#include <cstdarg>
#include <cstdint>
#include <print>
#include <thread>


namespace engine3d
{
    static void TraceImpl(const char *inFMT, ...)
    {
        // Format the message
        va_list list;
        va_start(list, inFMT);
        char buffer[1024];
        vsnprintf(buffer, sizeof(buffer), inFMT, list);
        va_end(list);

        // Print to the TTY
        std::print("{0}\n",buffer);
    }

    JoltHandler::JoltHandler()
    {
        // Set up the allocator
        JPH::RegisterDefaultAllocator();
        std::print("Wokring1\n");
        //Trace and assert callbacks
        JPH::Trace = TraceImpl;
	    JPH_IF_ENABLE_ASSERTS(AssertFailed = AssertFailedImpl;)
        std::print("Wokring2\n");
        // Create the factory
        JPH::Factory::sInstance = new JPH::Factory();
        std::print("Wokring2.5\n");

        assert(JPH::Factory::sInstance != nullptr && "Factory instance must be initialized");
        // Set up all types and shapes
        //! @note If we want to create our own shapes
        //! @note we have to register to collision dispatch first
        JPH::RegisterTypes();
        std::print("Wokring3\n");
        // Allocate data
        JPH::TempAllocatorImpl temp_allocator(100 * 1024 * 1024);
        std::print("Wokring3.5\n");

        // Example threadpool, branches will take care of this later.
        JPH::JobSystemThreadPool job_system(
            JPH::cMaxPhysicsJobs,
            JPH::cMaxPhysicsBarriers,
            std::thread::hardware_concurrency() - 1);

        // Max RB to exsist at once
        const uint32_t cMaxBodies = 60000;

        // Mutexes per body (default 0?)
        const uint32_t cNumBodyMutexes = 0;

        // Size of queue of pair checking
        const uint32_t cMaxBodyPairs = 60000;

        // Max number of touches per object
        const uint32_t cMaxContactConstraints = 16000;

        // Substructs for seperating types of bodies interface
        BPLayerInterfaceHandler broad_phase_layer_interface;

        // Filter for body types filter using broadphase
        ObjectVsBPLayerFilterInterface object_vs_broadphase_layer_filter;

        // Filter for body types using Object Layers
        ObjectLayerPairFilterInterface object_vs_object_layer_filter;

        std::print("Wokring4\n");
        // Actual physics system
        JPH::PhysicsSystem physics_system;
        physics_system.Init(
            cMaxBodies,
            cNumBodyMutexes,
            cMaxBodyPairs, 
            cMaxContactConstraints,
            broad_phase_layer_interface,
            object_vs_broadphase_layer_filter,
            object_vs_object_layer_filter);

        // Setting Contact listener
        Engine3DContactlistener contact_listener;
        physics_system.SetContactListener(&contact_listener);

        // Setting Activation listener
        Engine3DActivationListener body_activation_listener;
        physics_system.SetBodyActivationListener(&body_activation_listener);
    }

    JoltHandler::~JoltHandler()
    {
        JPH::UnregisterTypes();
        std::print("Deleting Physics Factory...\n");
        delete JPH::Factory::sInstance;
        JPH::Factory::sInstance = nullptr;
    }


};