#include <core/update_handlers/sync_update.hpp>
#include <core/engine_logger.hpp>
#include <core/update_handlers/global_update.hpp>
#include <core/application_instance.hpp>
#include "EditorWorld.hpp"
#include <renderer/renderer.hpp>

namespace engine3d{
    class Application : public ApplicationInstance{
    public:
        Application(const std::string& p_Tag) : ApplicationInstance(p_Tag){
            ConsoleLogFatal("Application::Application(std::string) gets called!");
            m_World = CreateRef<EditorWorld>("Editor World");

            GlobalUpdate::SubscribeApplicationUpdate(this, &Application::OnApplicationUpdate);

        }

        void OnApplicationUpdate(){
            //! TODO: Currently renderer is being called per-level. This is not what we want.
            //! @note What needs to happen is we provide a scene renderer.
            /*
            Scene Renderer
            - Handles multiple-passes for us.
            - Uses the draw call API's from the renderer to interface with the vulkan's API to do specific operations
            - Whether this is drawing 2D line, 3D objects, telling which uniforms to update, etc
            */
            m_World->OnUpdate();
        }

    private:
        Ref<EditorWorld> m_World;
    };

    Ref<ApplicationInstance> Initialize(){
        return CreateRef<Application>("Editor");
    }
};