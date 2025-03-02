#include <physics/jolt-cpp/jolt_api.hpp>
#include <physics/jolt-cpp/jolt-imports.hpp>
#include <cstdarg>
#include <core/engine_logger.hpp>

namespace atlas::physics {
    static void TraceImpl(const char* Message, ...) {
        va_list list;
        va_start(list, Message);
        char buffer[1024];
        vsnprintf(buffer, sizeof(buffer), Message, list);
        va_end(list);
        console_log_warn("TraceImpl Warning Occured!");
        console_log_warn("{}", buffer);
    }

    static ref<JPH::TempAllocatorImpl> temp_allocator = nullptr;
    static ref<JPH::JobSystemThreadPool> job_system;

    static JPH::PhysicsSystem physics_system;
    void JoltAPI::InitializeJoltAPI() {
        console_log_trace("Initiating JoltAPI!!!");
        JPH::RegisterDefaultAllocator();
        JPH::Trace = TraceImpl;

        JPH_IF_ENABLE_ASSERTS(AssertFailed = AssertFailedImpl;)

        // Create a factory, this class is responsible for creating instances of
        // classes based on their name or hash and is mainly used for
        // deserialization of saved data. It is not directly used in this
        // example but still required.
        JPH::Factory::sInstance = new JPH::Factory();

        JPH::RegisterTypes();

        temp_allocator = create_ref<JPH::TempAllocatorImpl>(10 * 1024 * 1024);
        job_system = create_ref<JPH::JobSystemThreadPool>(
          JPH::cMaxPhysicsJobs,
          JPH::cMaxPhysicsBarriers,
          std::thread::hardware_concurrency() - 1);

        // This is the max amount of rigid bodies that you can add to the
        // physics system. If you try to add more you'll get an error. Note:
        // This value is low because this is a simple test. For a real project
        // use something in the order of 65536.
        const uint32_t cMaxBodies = 1024;

        // This determines how many mutexes to allocate to protect rigid bodies
        // from concurrent access. Set it to 0 for the default settings.
        const uint32_t cNumBodyMutexes = 0;

        // This is the max amount of body pairs that can be queued at any time
        // (the broad phase will detect overlapping body pairs based on their
        // bounding boxes and will insert them into a queue for the
        // narrowphase). If you make this buffer too small the queue will fill
        // up and the broad phase jobs will start to do narrow phase work. This
        // is slightly less efficient. Note: This value is low because this is a
        // simple test. For a real project use something in the order of 65536.
        const uint32_t cMaxBodyPairs = 1024;

        // This is the maximum size of the contact constraint buffer. If more
        // contacts (collisions between bodies) are detected than this number
        // then these contacts will be ignored and bodies will start
        // interpenetrating / fall through the world. Note: This value is low
        // because this is a simple test. For a real project use something in
        // the order of 10240.
        const uint32_t cMaxContactConstraints = 1024;

        // Create mapping table from object layer to broadphase layer
        // Note: As this is an interface, PhysicsSystem will take a reference to
        // this so this instance needs to stay alive! Also have a look at
        // BroadPhaseLayerInterfaceTable or BroadPhaseLayerInterfaceMask for a
        // simpler interface.
        BPLayerInterfaceHandler broad_phase_layer_interface;

        // Create class that filters object vs broadphase layers
        // Note: As this is an interface, PhysicsSystem will take a reference to
        // this so this instance needs to stay alive! Also have a look at
        // ObjectVsBroadPhaseLayerFilterTable or
        // ObjectVsBroadPhaseLayerFilterMask for a simpler interface.
        ObjectVsBPLayerFilterInterface object_vs_broadphase_layer_filter;

        // Create class that filters object vs object layers
        // Note: As this is an interface, PhysicsSystem will take a reference to
        // this so this instance needs to stay alive! Also have a look at
        // ObjectLayerPairFilterTable or ObjectLayerPairFilterMask for a simpler
        // interface.
        ObjectLayerPairFilterInterface object_vs_object_layer_filter;

        physics_system.Init(cMaxBodies,
                            cNumBodyMutexes,
                            cMaxBodyPairs,
                            cMaxContactConstraints,
                            broad_phase_layer_interface,
                            object_vs_broadphase_layer_filter,
                            object_vs_object_layer_filter);

        ActivationListener activation_listener;
        physics_system.SetBodyActivationListener(&activation_listener);

        ContactListener contact_listener;
        physics_system.SetContactListener(&contact_listener);

        console_log_error("JoltAPI::Initialize Successfuly!!!!!");
    }

    JPH::PhysicsSystem& JoltAPI::GetSystem() {
        return physics_system;
    }

    JPH::BodyInterface* JoltAPI::GetBodyInterface() {
        return &(physics_system.GetBodyInterface());
    }

};