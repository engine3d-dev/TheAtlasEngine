#pragma once
#include <engine3d/Core/Event/Event.h>

namespace engine3d{
    class Layer{
    public:
        Layer(const std::string& name="Default String");
        void OnAttach();
        void OnDetach();
        void OnUpdate(float ts);
        void OnEvent(Event& event);
        void OnUIRender();
        std::string GetLayerString() const;

    protected:
        virtual void BeginPlay() = 0;
        virtual void EndPlay() = 0;
        virtual void UpdateFrame(float ts) = 0;
        virtual void UpdateEvent(Event& event) = 0;
        virtual void UpdateUI() = 0;
    };
};