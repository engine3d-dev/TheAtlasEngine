#include "EngineLayer.h"
#include "engine3d/Core/EngineLogger.h"
#include "engine3d/Core/Event/InputPoll.h"

namespace engine3d{
    EngineLayer::EngineLayer() : Layer("EngineLayer") {}

    void EngineLayer::BeginPlay(){
        CoreLogInfo("EngineLayer::BeginPlay function called in class {}", Layer::GetLayerString());
    }

    void EngineLayer::EndPlay(){
        // CoreLogInfo("EngineLayer::EndPlay function called");
    }

    void EngineLayer::UpdateFrame(float ts){
        // CoreLogInfo("EngineLayer::UpdateFrame(float) function called");
        if(InputPoll::IsKeyPressed(ENGINE_KEY_ESCAPE)){
            exit(0);
        }
    }

    void EngineLayer::UpdateEvent(Event& event){
        // CoreLogInfo("EngineLayer::UpdateEvent(Event&) function called");
    }

    void EngineLayer::UpdateUI(){}


    std::string EngineLayer::GetName() const { return Layer::GetLayerString(); }
};