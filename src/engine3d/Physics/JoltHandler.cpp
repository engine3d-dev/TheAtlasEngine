#include <Jolt/Core/IssueReporting.h>
#include <Physics/JoltHandler.hpp>
namespace engine3d
{
    JoltHandler::JoltHandler()
    {
        // Set up the allocator
        JPH::RegisterDefaultAllocator();

        // Trace and assert callbacks
        
	    JPH_IF_ENABLE_ASSERTS(AssertFailed = AssertFailedImpl;)


    }
};