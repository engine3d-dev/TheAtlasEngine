#include "Editor.h"
#include <engine3d/Core/Event/InputPoll.h>
#include "engine3d/Core/EngineLogger.h"
#include "engine3d/Core/Renderer/Renderer.h"
#include <engine3d/Core/Timestep.h>
#include <imgui/imgui.h>

namespace engine3d{

    EditorApplication::EditorApplication(const std::string& p_DebugName) : ApplicationInstance(p_DebugName) {
<<<<<<< HEAD
        Renderer::Initialize();
        Renderer::SetBackgroundColor({1.0f, 0.0f, 0.0f, 0.0f});
    }

    EditorApplication::~EditorApplication() {}

    void EditorApplication::UpdateThisApplicationInstance(){
        
        //! @note Just testing to see if application still closes cleanly.
        // if(InputPoll::IsKeyPressed(ENGINE_KEY_ESCAPE)){
        //     exit(0);
        // }

        //! @note This function will render our primitives
        //! @note TODO --  Flush should only happens when our scene is given everything that lives within this scene (ref to lifetimes)
        /* Renderer::FlushScene(); */
		
		Renderer::Presentation();
        // ImGui::Begin("Hello, world!"); // Create a window called "Hello, world!" and append into it.
        // ImGui::End();
    }

    ApplicationInstance* InitializeApplication(){
        return new EditorApplication();
    }

=======
        /* Renderer::Initialize(); */
        /* Renderer::SetBackgroundColor({0.0f, 0.5f, 0.5f, 0.f}); */
        Renderer::SetBackgroundColor({1.0f, 0.0f, 0.0f, 0.0f});

        ConsoleLogInfo("Initializing Current Application Name === {}!!!", ApplicationInstance::CurrentApplicationName());
    }

    EditorApplication::~EditorApplication() {}

    void EditorApplication::UpdateThisApplicationInstance(){
        
        //! @note Just testing to see if application still closes cleanly.
        // if(InputPoll::IsKeyPressed(ENGINE_KEY_ESCAPE)){
        //     exit(0);
        // }

        //! @note This function will render our primitives
        //! @note TODO --  Flush should only happens when our scene is given everything that lives within this scene (ref to lifetimes)
        /* Renderer::FlushScene(); */
		
		Renderer::Presentation();
        // ImGui::Begin("Hello, world!"); // Create a window called "Hello, world!" and append into it.
        // ImGui::End();
    }

    ApplicationInstance* InitializeApplication(){
        return new EditorApplication();
    }

>>>>>>> fa59aaf07c9cbf80f5d25918c4d43b3cca25238f


};
