
#include <engine3d/Core/EngineLogger.h>
#include <engine3d/Core/ApplicationInstance.h>
#include <engine3d/Core/Renderer/Renderer.h>

namespace engine3d{
    class TestApplication : public ApplicationInstance{
    public:
        TestApplication(const std::string p_DebugName="TestApplication") : ApplicationInstance(p_DebugName) {
            Renderer::Initialize();
            Renderer::SetBackgroundColor({0.0f, 0.5f, 0.5f, 0.f});
        }

        void UpdateThisApplicationInstance(){
            ConsoleLogInfo("Running application!");
        
        //! @note Just testing to see if application still closes cleanly.
        // if(InputPoll::IsKeyPressed(ENGINE_KEY_ESCAPE)){
        //     exit(0);
        // }

        //! @note This function will render our primitives
        //! @note TODO --  Flush should only happens when our scene is given everything that lives within this scene (ref to lifetimes)
        Renderer::FlushScene();

        // ImGui::Begin("Hello, world!"); // Create a window called "Hello, world!" and append into it.
        // ImGui::End();
    }

    };

     ApplicationInstance* InitializeApplication(){
        return new TestApplication();
    }
};