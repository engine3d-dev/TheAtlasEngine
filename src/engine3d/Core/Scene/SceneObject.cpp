#include <Core/Scene/SceneObject.hpp>

namespace engine3d{
    SceneObject SceneObject::Create(){
        static id_t current_id = 0;
        return SceneObject(current_id);
    }

    SceneObject::SceneObject(id_t p_Id) : m_Id(p_Id) {}
};