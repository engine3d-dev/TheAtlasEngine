#include "EngineLayer.h"
#include <engine3d/Core/EngineLogger.h>
#include <engine3d/Core/Event/InputPoll.h>
#include <engine3d/Core/Event/KeyEvent.h>

namespace engine3d{
    EngineLayer::EngineLayer() : Layer("EngineLayer") {}

    void EngineLayer::BeginPlay(){
        ConsoleLogInfo("EngineLayer::BeginPlay function called in class {}", Layer::GetLayerString());
    }

    void EngineLayer::EndPlay(){
        // ConsoleLogInfo("EngineLayer::EndPlay function called");
    }

    void EngineLayer::UpdateFrame(float ts){
        // ConsoleLogInfo("EngineLayer::UpdateFrame(float) function called");
        if(InputPoll::IsKeyPressed(ENGINE_KEY_ESCAPE)){
            ConsoleLogError("IsKeyPressed pressed!");
            exit(0);
        }
    }

    // void EngineLayer::UpdateEvent(Event& event){
    //     // ConsoleLogInfo("EngineLayer::UpdateEvent(Event&) function called");
    // }

    void EngineLayer::UpdateUI(){}


    std::string EngineLayer::GetName() const { return Layer::GetLayerString(); }
};