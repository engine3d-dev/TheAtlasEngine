#include <core/update_handlers/sync_update_manager.hpp>
#include <core/engine_logger.hpp>
#include <core/update_handlers/global_update_manager.hpp>
#include <core/application_instance.hpp>
#include "EditorWorld.hpp"
#include <renderer/renderer.hpp>

namespace engine3d{
    class Application : public engine3d::ApplicationInstance{
    public:
        Application(const std::string& p_Tag) : engine3d::ApplicationInstance(p_Tag), m_World("Untitled"){
            GlobalUpdateManager::SubscribeApplicationUpdate(this, &Application::OnApplicationUpdate);
            ConsoleLogFatal("Applicatoin::Application(std::string) gets called!");
            // m_World = CreateRef<EditorWorld>();
            m_World.OnStart();
        }

        void OnApplicationUpdate(){
            // ConsoleLogInfoWithTag("Editor", "OnUpdate called from application!");
            
            //! @note For now, I am going to use this for specifically rendering
            //! @note Just to test that we can still render things.
            m_World.OnUpdate();


            //! @note Rendering happens here
            // Renderer::RenderSceneObjects(m_World.GetAllWorldObjects());
        }


        void OnUpdate(){
            // ConsoleLogInfoWithTag("Editor", "OnApplicationUpdate called from application!");
        }

    private:
        EditorWorld m_World;
    };

    Ref<ApplicationInstance> Initialize(){
        return CreateRef<Application>("Editor");
    }
};