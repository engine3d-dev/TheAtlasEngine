#pragma once

//! @note Jolt is required include before including anything jolt-related
//! @note We can also treat this as a way to precompile-headers for specifically jolt physics abstraction layer
// #ifndef JPH_DEBUG_RENDERER
// #define JPH_DEBUG_RENDERER
// #endif
// #define JPH_SHARED_LIBRARY 
// #define JPH_PROFILE_ENABLED 0

//! @note ALMOST THEREEE!!! Look at conan file and update cmake utils on caching those compiler variables
// #ifndef JPH_PROFILE_ENABLED
// #define JPH_PROFILE_ENABLED 0
// #endif
// #define JPH_PROFILE_ENABLED 
// #define JPH_PROFILE_ENABLED
// #define JPH_FLOATING_POINT_EXCEPTIONS_ENABLED
#include <Jolt/Jolt.h>
#include <Jolt/Core/Core.h>
#include <Jolt/Physics/Body/BodyInterface.h>
#include <Jolt/Core/JobSystemThreadPool.h>
#include <Jolt/Physics/PhysicsSettings.h>
#include <Jolt/Core/IssueReporting.h>
#include <Jolt/Core/TempAllocator.h>
#include <Jolt/RegisterTypes.h>
#include <Jolt/Core/Factory.h>
#include <Jolt/Core/Memory.h>
#include <Jolt/Physics/Collision/ContactListener.h>
#include <Jolt/Physics/PhysicsSystem.h>
#include <Jolt/Physics/Collision/ObjectLayer.h>
#include <Jolt/Physics/Collision/BroadPhase/BroadPhaseLayer.h>
#include <Jolt/Core/IssueReporting.h>

#include <Jolt/Physics/Body/BodyCreationSettings.h>

#include <Jolt/Physics/Collision/Shape/Shape.h>
#include <Jolt/Physics/Collision/Shape/BoxShape.h>
#include <Jolt/Physics/Collision/Shape/ConvexShape.h>
#include <Jolt/Physics/Collision/Shape/ScaledShape.h>
#include <Jolt/Physics/Collision/Shape/SphereShape.h>

#include <Jolt/Physics/EActivation.h>

// jolt's math includes
#include <Jolt/Math/MathTypes.h>
#include <Jolt/Math/Real.h>
#include <Jolt/Math/Quat.h>

// Disable common warnings triggered by Jolt, you can use JPH_SUPPRESS_WARNING_PUSH / JPH_SUPPRESS_WARNING_POP to store and restore the warning state
// JPH_SUPPRESS_WARNINGS













// Jolt includes
// #include <Jolt/RegisterTypes.h>
// #include <Jolt/Core/Factory.h>
// #include <Jolt/Core/TempAllocator.h>
// #include <Jolt/Core/JobSystemThreadPool.h>
// #include <Jolt/Physics/PhysicsSettings.h>
// #include <Jolt/Physics/PhysicsSystem.h>
// #include <Jolt/Physics/Collision/Shape/BoxShape.h>
// #include <Jolt/Physics/Collision/Shape/SphereShape.h>
// #include <Jolt/Physics/Body/BodyCreationSettings.h>
// #include <Jolt/Physics/Body/BodyActivationListener.h>

// #include <Jolt/Physics/Body/MotionType.h>
// #include <Jolt/Physics/Collision/ObjectLayer.h>