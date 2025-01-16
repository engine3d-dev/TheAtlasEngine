#include "EditorWorld.hpp"
#include <core/engine_logger.hpp>
#include <scene/components/components.hpp>


namespace engine3d{
    EditorWorld::EditorWorld(){
        ConsoleLogFatal("Instantiate Default EditorWorld!");
    }

    EditorWorld::EditorWorld(const std::string& p_Tag) : m_Tag(p_Tag){
        m_MainWorld = engine3d::WorldScope("Main World");

        ConsoleLogTrace("m_MainWorld->GetTag() = {}", m_MainWorld.GetTag());

        m_FirstLevelScene = engine3d::CreateRef<LevelScene>("LevelScene");
        m_MainWorld.AddScene(m_FirstLevelScene);


    }


    //! TODO: Implement either a scene graph, or decide the data structure to go with in how we want to handle scenes
    void EditorWorld::OnUpdate(){
        m_MainWorld.OnUpdate();
    }
};