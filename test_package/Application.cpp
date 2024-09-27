#include "Application.h"
#include <Core/Event/InputPoll.h>
#include "Core/EngineLogger.h"
#include "Core/Renderer/Renderer.h"
#include <engine3d/Core/Timestep.h>
// #include <imgui/imgui.h>

namespace engine3d{

    TestApplication::TestApplication(const std::string& p_DebugName) : ApplicationInstance(p_DebugName) {
        Renderer::Initialize();
        Renderer::SetBackgroundColor({0.0f, 0.5f, 0.5f, 0.f});

        ConsoleLogInfo("Initializing Current Application Name === {}!!!", ApplicationInstance::CurrentApplicationName());
    }

    TestApplication::~TestApplication() {}

    void TestApplication::UpdateThisApplicationInstance(){
        Renderer::RenderBasicClearColor();
        Renderer::Presentation();
    }

    ApplicationInstance* InitializeApplication(){
        return new TestApplication();
    }



};
