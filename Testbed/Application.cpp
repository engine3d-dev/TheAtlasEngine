#include "Application.h"
#include <engine3d/Core/Renderer/Renderer.h>

namespace engine3d{
    TestbedApplication::TestbedApplication(const std::string& p_DebugName){
        Renderer::Initialize();
        Renderer::SetBackgroundColor({1.0f, 0.0f, 0.0f, 0.0f});
    }

    void TestbedApplication::UpdateThisApplicationInstance(){
        Renderer::Presentation();
    }
};