#pragma once
// #include <Core/Event/Event.h>
#include <string>

namespace engine3d{
    /**
     * @name Layer
     * @note TODO -- Call this world or something.
     * @note Layers are how we specify different levels/worlds
     * @param TODO, Activate() - is a function for checking if we ticked this current world to be activated
     * @note Also enabling us to toggle which worlds/levels we want to activate/render.
    */
    class Layer{
    public:
        Layer(const std::string& name="Default String");
        void OnAttach();
        void OnDetach();
        void OnUpdate(float ts);
        // void OnEvent(Event& event);
        void OnUIRender();
        std::string GetLayerString() const;

        //! @note Checking for toggling if our current world is activated or not.
        // bool IsActive() const;

    protected:
        virtual void BeginPlay() = 0;
        virtual void EndPlay() = 0;
        virtual void UpdateFrame(float ts) = 0;
        // virtual void UpdateEvent(Event& event) = 0;
        virtual void UpdateUI() = 0;
    };
};