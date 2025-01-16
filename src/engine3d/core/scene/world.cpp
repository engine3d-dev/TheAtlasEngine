#include <core/scene/world.hpp>
#include <core/system_framework/system_registry.hpp>
#include <core/scene/scene.hpp>
namespace engine3d{

    WorldScope::WorldScope(const std::string& p_Tag){
        ConsoleLogInfo("WorldScope::WorldScope(std::string Tag) = {}", m_Tag);
        ConsoleLogFatal("Just registered world using SystemRegistry::Register(this)");
        m_Tag = p_Tag;

        //! @note This means that when someone instantiates a world scope that gets registers immediately at initialization
        //! @note What we could do is have this be registerd OnLoad(bool) where the bool lets user define whether to register this world
        SystemRegistry::Register(this);
    }
    
    /*
        Provides a way of passing scene context
        This is just to test SystemRegistry and how registering worlds/scenes would work
        @note Really, this would be providing a way of handling scenes in some spatial data structure
    */
    void WorldScope::AddScene(Ref<SceneScope> p_SceneContext){
        m_CurrentScene = p_SceneContext;
        m_SceneManager.push_back(m_CurrentScene);
    }

    /*
        This OnUpdate is just for testing purposes. Sole purpose for switching this out with a better methodology

        This could be replaced by that data structure or some way at updating active scenes specifically
        Just so we are not updating every scene unecessarily, meaning updating only in the view and nearby locations of the player
    */
    void WorldScope::OnUpdate(){
        m_CurrentScene->OnUpdate();
    }
};