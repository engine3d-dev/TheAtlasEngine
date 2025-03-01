#include <physics/physics_scene.hpp>
#include <physics/jolt-cpp/helper_functions.hpp>
#include <physics/jolt-cpp/jolt_api.hpp>

namespace atlas::physics{
    physics_scene::physics_scene(const scene_scope* p_current_scene_ctx){
        // m_PhysicsSystem = (JPH::PhysicsSystem*)p_current_physics_sys;
        m_current_scene_ctx = create_scope<scene_scope>(*p_current_scene_ctx);
    }

    void physics_scene::on_runtime_start(){
        m_is_physics_enabled = true;
        const uint32_t cMaxBodies = 1024;
        const uint32_t cNumBodyMutexes = 0;
        const uint32_t cMaxBodyPairs = 1024;
        const uint32_t cMaxContactConstraints = 1024;
        physics::BPLayerInterfaceHandler broad_phase_layer_interface;
        physics::ObjectVsBPLayerFilterInterface object_vs_broadphase_layer_filter;
        physics::ObjectLayerPairFilterInterface object_vs_object_layer_filter;

        // JPH::PhysicsSystem physics_system;
        m_physics_system = create_ref<JPH::PhysicsSystem>();
        m_physics_system->Init(cMaxBodies, cNumBodyMutexes, cMaxBodyPairs, cMaxContactConstraints, broad_phase_layer_interface, object_vs_broadphase_layer_filter, object_vs_object_layer_filter);
        
        physics::ActivationListener activation_listener;
        m_physics_system->SetBodyActivationListener(&activation_listener);

        physics::ContactListener contact_listener;
        m_physics_system->SetContactListener(&contact_listener);

        console_log_error("JoltAPI::Initialize Successfuly!!!!!");


        JPH::BoxShapeSettings floor_shape_settings(JPH::Vec3(100.0f, 1.0f, 100.0f));
        floor_shape_settings.SetEmbedded();

        // Creating shape
        JPH::ShapeSettings::ShapeResult floor_shape_result = floor_shape_settings.Create();
        JPH::ShapeRefC floor_shape = floor_shape_result.Get();

        // Creating setting for body itself

        JPH::RVec3 obj_inPosition = JPH::RVec3(0.0, 0.0, 1.0);
        // auto sphere_position = m_Sphere->get<Transform>()->Position;
        // JPH::RVec3 inPosition = JPH::RVec3(physics::to_rvec3(sphere_position));
        JPH::RVec3 inPosition = obj_inPosition;
        JPH::Quat inRotation = JPH::Quat::sIdentity();
        JPH::EMotionType inMotionType = JPH::EMotionType::Static;
        JPH::ObjectLayer bodyType = physics::BodyTypeTest::STATIC;

        JPH::BodyCreationSettings floor_settings(floor_shape, inPosition, inRotation, inMotionType, bodyType);

        auto& body_interface = m_physics_system->GetBodyInterface();
        JPH::Body* floor = body_interface.CreateBody(floor_settings);

        body_interface.AddBody(floor->GetID(), JPH::EActivation::DontActivate);
    }

    void physics_scene::on_runtime_stop(){

        if(m_is_physics_enabled){
            m_is_physics_enabled = false;
            m_current_scene_ctx = nullptr;
            console_log_trace("m_current_scene_ctx IS nullptr!!!");
        }

    }

    void add_scene_object([[maybe_unused]] const ref<scene_object>& p_current_object){}

    void physics_scene::on_runtime_update([[maybe_unused]] float physics_dt){
        if(!m_is_physics_enabled){
            return;
        }

        auto* world = system_registry::get_world().get_registry();

        //! @note We are going to query all of the entities within our scene and check if they all have a RigidBody3D component associated with them
        //! @note In flecs we query what component we want to check for all our entities that are stored in flecs::world
        auto all_entities = world->query<RigidBody3D>();

        // We are going through all those entities with this specific component and we want to start the physics simulation if those entities have these specific components
        all_entities.each([]([[maybe_unused]] RigidBody3D& p_physic_body){
            console_log_trace("Called because RigidBody3D found!!!");
        });

        // console_log_trace("physics_scene::on_runtime_update Called!!!");
    }
};
