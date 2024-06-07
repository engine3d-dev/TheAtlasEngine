#pragma once
#include <engine3d/Core/core.h>
#include <engine3d/Core/Layer.h>


namespace Engine3D{

    // Essentially LayerStack is a wrapper of std::vectors of Layers
    // using vector so we can iterate over.
    // for updates we need to iterate through it
    // Not using Stack because we eventually wanna store layers in the middle.
    // So, like some contiguous stack of layers.
    class ENGINE_API LayerStack{
        using iterator = std::vector<Layer *>::iterator;
    public:
        LayerStack();
        ~LayerStack();

        void PushLayer(Layer* layer);
        void PushOverlay(Layer* overlay);
        void PopLayer(Layer* layer);
        void PopOverlay(Layer* overlay);

        iterator begin() { return m_Layers.begin(); }
        iterator end() { return m_Layers.end(); }
    private:
        std::vector<Layer *> m_Layers;
       unsigned int m_LayersInsertIndex = 0;
    };
};