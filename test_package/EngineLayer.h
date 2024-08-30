#pragma once
#include <engine3d/Core/Layer.h>

namespace engine3d{
    //! @note THis is where we do our actual engine interactiveness for the editor here.
    class EngineLayer : public Layer{
    public:
        EngineLayer();
        //! @note To get our current world layer.
        //! @note Abstracting where the user is trying to add widgets to the engine or their game.
        //! @note EngineLayer ideally could be the actors known world view.
        std::string GetName() const;
    protected:
        void BeginPlay() override;
        void EndPlay() override;
        void UpdateFrame(float ts) override;
        // void UpdateEvent(Event& event) override;
        void UpdateUI() override;
    };
};