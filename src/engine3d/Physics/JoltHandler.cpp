#include <Physics/JoltHandler.hpp>
// Other includes
#include <cassert>
#include <cstdarg>
#include <cstdint>
#include <print>
#include <thread>
//! @note All includes under here except JoltHandler



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

        TempAllocatorImpl physics_allocator(100 * 1024 * 1024);

        assert(JPH::Factory::sInstance != nullptr && "Factory instance must be initialized");
        // Set up all types and shapes
        //! @note If we want to create our own shapes
        //! @note we have to register to collision dispatch first
        JPH::RegisterTypes();
        std::print("Wokring3\n");

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

        physics_system.SetContactListener(&contact_listener);

        physics_system.SetBodyActivationListener(&body_activation_listener);

        BodyInterface &body_interface = physics_system.GetBodyInterface();

        
    }

    JoltHandler::~JoltHandler()
    {
        JPH::UnregisterTypes();
        std::print("Deleting Physics Factory...\n");
        delete JPH::Factory::sInstance;
        JPH::Factory::sInstance = nullptr;
    }


};