#include "Editor.hpp"
#include "Core/Event/InputPoll.hpp"
#include "Core/Event/KeyCodes.hpp"
#include <engine3d/Core/EngineLogger.hpp>
#include <engine3d/Core/Timestep.hpp>
namespace engine3d{

    EditorApplication::EditorApplication(const std::string& p_DebugName) : ApplicationInstance(p_DebugName) {
        // Renderer::Initialize();
        // Renderer::SetBackgroundColor({1.0f, 0.0f, 0.0f, 0.0f});
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
		
		// Renderer::Presentation();
        // ImGui::Begin("Hello, world!"); // Create a window called "Hello, world!" and append into it.
        // ImGui::End();
    }

    ApplicationInstance* InitializeApplication(){
        return new EditorApplication();
    }



};
