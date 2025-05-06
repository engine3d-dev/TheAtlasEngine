#include <engine_logger.hpp>
#include <core/scene/scene.hpp>
#include <Jolt/Jolt.h>
#include <Jolt/Physics/Body/Body.h>
#include <physics/jolt-cpp/jolt_contact_listener.hpp>

#include <flecs.h>

namespace atlas::physics {

JPH::ValidateResult contact_listener::OnContactValidate(const JPH::Body&, const JPH::Body&,
                                        JPH::RVec3Arg, const JPH::CollideShapeResult&){
    return JPH::ValidateResult::AcceptContact;
}

void contact_listener::OnContactAdded(const JPH::Body& body1, const JPH::Body& body2,
                    const JPH::ContactManifold&, JPH::ContactSettings&){

    ref<world_scope> world_object =
    system_registry::get_world("Editor World");
    if (!world_object) {
        console_log_error("World not found.");
        return;
    }

    ref<scene_scope> scene = world_object->get_scene("LevelScene");
    if (!scene) {
        console_log_error("Scene not found.");
        return;
    }
    auto entity1 = scene->query_builder<>()


    auto entity2 = body_to_entity.find(body2.GetID());

    if (it1 != body_to_entity.end() && it2 != body_to_entity.end()) {
        m_contact_list.push_back({body1.GetID(),body2.GetID()});
    }


}


void contact_listener::OnContactPersisted(const JPH::Body&, const JPH::Body&,
                        const JPH::ContactManifold&, JPH::ContactSettings&) {

    //!  @note We dont need this for now so I am going to ignore it
    return;
}

void contact_listener::OnContactRemoved(const JPH::SubShapeIDPair&) {
    //! @note We dont need this for now so I am going to ignore it
    return;
}



}
