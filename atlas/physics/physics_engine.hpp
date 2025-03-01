#pragma once
#include <core/core.hpp>
#include <physics/jolt-cpp/jolt-imports.hpp>
#include <Jolt/Physics/Collision/ObjectLayer.h>
#include <Jolt/Physics/Body/BodyManager.h>
#include <Jolt/Physics/Body/BodyActivationListener.h>
#include <Jolt/Physics/Collision/ContactListener.h>
#include <array>

namespace atlas::physics{
    /**
     * @note Here are a few things to do
     * TODO: Handle various entry points to a scene. Such as physics_scene (which essentially just wraps our scene)
     * TODO: Looking at how to handle different scene physics. For ensuring that when we transition to a different scene the physics isnt using data from previous scene
     * TODO: timer namespace, specifically for having different timers
     *      - timer for physics
     *      - timer for rendering (may utilize the same timer that is used throughout the engine)
     * @note This is because physics timers need have to be faster then the timer the rendering uses
     * 
     * 
     * 
     * @name Physics Engine
     * @note Engine3D's abstraction layer for specifically porting over jolt's or potentially any other physics framework into engine3d
     * 
     * 
     * Here are where I believe a physics system should reside
     * 
     * Scenes
     *  - Having some class thats called PhysicScene. Physics Scene is responsible for the scene's physics
     *  - Each of these "PhysicScene" contains something called physics::settings
     *  - physics::setting contains properties such as gravity and other environmental parameters that can be modified for that specific targeted scene
    */

    //! @note Used for indicating whether objects in a given scene are static or dynamic (meaning can collide with other objects)
    //! @note Used as a representation for both ObjectLayer(uint16_t) and BroadPhaseLayer(class)

    /**
     * @note ObjectLayer just seems to be defined as a body type within joltphysics
     * @note BroadPhaseLayer is just joltphysic's representation of a collider pretty much. It is mainly used by the jolt physics collision
     *       detection system that uses it. Typically in JoltPhysics you'd have less of these then ObjectLayer's it seems
     * 
     * @note BroadPhaseLayer can be ways to map multiple object layers (BodyType's) 
    */
    //! @note Physics settings per physic scene
    // struct setting{
    //     float gravity = -10.f;
    // };


    /**
       @name PhysicSystem
       @note Wrapper around Jolt's API for initiating the physics system
       @note These are exmaples of the API's
       
       

       Physic Body
        - Will be a component purely containing the data that will be translated to Jolt
        - PhysicBody is a component that'll be added so when physic's enabled then we can
            use this component to work with basic physic properties. Like applying gravity


        PhysicSystem
            - Responsible for managing the physics objects
            - Responsible for handling per scene's physics setting
            - Each scene in itself will contain something called PhysicScene
            - PhysicScene will be overlayed on top of the scene. This is how that scene will have physics
                - We also want scenes to have independent physics settings so they are not using previous data from the other scene's physics
        
        //! @note These are notes for myself on triggers. Just so I dont forget. Lets not think about this for the time being.

        Here is an idea for handling triggering events

        Utilize Event Dispatcher System

        Example API's

        Alternative #1

        // Idea behind this is we get our event dispatcher. Then we go ahead and submit a trigerred event when sphere activates that specific event
        // IDEA: We need to figure out a way to target specific objects and how they are able to submit triggering events so the event system is aware it is happening
        auto dispatcher = event::get_event();
        dispatcher.dispatch<SphereTriggered>(sphere_object);
    */
    // class PhysicSystem{
    // public:
    //     //! @note Constructor will instantiate the physics sysytem
    //     //! @note NOTE: Apply dedicated threads for this physic system
    //     // PhysicSystem(const setting& p_Settings = {});
        
    //     static JPH::PhysicsSystem& GetInstance();

    //     // static ref<JPH::Body> CreateBody(const JPH::BodyCreationSettings& settings);
    //     static JPH::BodyInterface& CreateBodyInterface();

    //     static void OptimizeBroadPhase();
    //     // void Init();
    // };
};  