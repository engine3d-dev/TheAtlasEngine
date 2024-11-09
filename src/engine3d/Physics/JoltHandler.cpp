#include <Physics/JoltHandler.hpp>
#include <Physics/BPLayerInterfaceHandler.hpp>
//! @note All includes under here except JoltHandler
#include <Jolt/Core/JobSystemThreadPool.h>
#include <Jolt/Physics/PhysicsSettings.h>
#include <Jolt/Core/IssueReporting.h>
#include <Jolt/Core/TempAllocator.h>
#include <Jolt/RegisterTypes.h>
#include <Jolt/Core/Factory.h>
#include <Jolt/Core/Memory.h>
#include <cstdint>
#include <thread>


namespace engine3d
{
    static void TraceImpl(const char *inFMT, ...){}

    JoltHandler::JoltHandler()
    {
        // Set up the allocator
        JPH::RegisterDefaultAllocator();

        //Trace and assert callbacks
        JPH::Trace = TraceImpl;
	    JPH_IF_ENABLE_ASSERTS(AssertFailed = AssertFailedImpl;)

        // Create the factory
        JPH::Factory::sInstance = new JPH::Factory();

        // Set up all types and shapes
        //! @note If we want to create our own shapes
        //! @note we have to register to collision dispatch first
        JPH::RegisterTypes();

        // Allocate data
        JPH::TempAllocatorImpl temp_allocator(100 * 1024 * 1024);

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

        // Substructs for seperating types of bodies
        BPLayerInterfaceHandler broad_phase_layer_interface;


    }

    JoltHandler::~JoltHandler()
    {
        JPH::UnregisterTypes();
        delete JPH::Factory::sInstance;
        JPH::Factory::sInstance = nullptr;
    }


};