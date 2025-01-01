#include <scene/scene.hpp>

namespace engine3d{
    Scene::Scene(const std::string& p_Tag) : m_Tag(p_Tag){}
        
    Scene::~Scene(){
    }

    EntityObject Scene::CreateEntity(const std::string& p_Tag){
        return EntityObject(&m_SceneRegistry, p_Tag);
    }


    void Scene::Test(){}
};