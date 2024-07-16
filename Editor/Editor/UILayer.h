#pragma once

#include "engine3d/Core/Layer.h"
namespace engine3d{
    class UILayer : public engine3d::Layer{
    public:
        UILayer();

        std::string GetName() const;
    protected:
        void BeginPlay() override;
        void EndPlay() override;
        void UpdateFrame(float ts) override;
        void UpdateEvent(Event& event) override;
        void UpdateUI() override;
    };
};