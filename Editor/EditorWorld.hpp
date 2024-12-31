#pragma once
#include <scene/scene_node.hpp>
#include <scene/scene.hpp>
#include <core/core.hpp>

/**
 * @name EditorScene
*/
class EditorWorld{
public:
    EditorWorld();
    EditorWorld(const std::string& p_Tag);

    void OnStart();

    void OnStop();


    void OnUpdate();

    //! @note These are going to be all render targets
    std::map<std::string, engine3d::Ref<engine3d::SceneNode>>& GetAllWorldObjects() { return m_SceneEntities; }

    //! TODO: We can move this somewhere
    //! @note For now, this is going to be called from the renderer
    //! @note Renderer::RenderScene(Scene) takes in a scene to render
    //! @note Eventually this is what the scene render will utilize.
    // engine3d::Ref<SceneNode> GetWorldScene() { return m_Scene1; }
    // engine3d::Ref<engine3d::SceneNode> GetOneSceneNode() { return new_node1; }
    // std::span<engine3d::SceneNode> GetAllSceneNodes() { return m_ChildrenSceneNodes; }
private:
    std::map<std::string, engine3d::Ref<engine3d::SceneNode>> m_SceneEntities;
    engine3d::Scene m_Scene1;
};