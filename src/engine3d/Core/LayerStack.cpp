#include <engine3d/engine3dPrecompiledHeader.h>
#include <engine3d/Core/LayerStack.h>

namespace Engine3D{
    LayerStack::LayerStack(){
    }

    LayerStack::~LayerStack(){
        for(Layer* layer : m_Layers){
            delete layer;
        }
    }

    // Pushing into the layer stack, and making sure your Pushing into the right place
    // Layers get Pushed to thhe first half of the list
    void LayerStack::PushLayer(Layer* layer){
        m_Layers.emplace(m_Layers.begin() + m_LayersInsertIndex, layer);
        m_LayersInsertIndex++;
    }

    // Overlays gets Pushhed into thhe second half of the list.
    void LayerStack::PushOverlay(Layer* overlay){
        m_Layers.emplace_back(overlay);
    }

    // How ownership works with layers, are kind of interesting.
    // When you Pop layers, you dont lose them, only when layers gets destroyed
    // - Another thing is that the Layer Stack is owned by the application, which means that 
    //  basically the way this works is that you can allocate your layer and as long you give it
    //  to the layer stack and don't take it out, thhey will be deallocated when the application shuts down
    // - Layers are meant to live throughout the lifetime of your application at this given point in time.
    // - In the future, if you create levels or anythhing like that then that is when it is worthhwhhile
    //   to recreate thhe entire layer stack
    void LayerStack::PopLayer(Layer* layer){

        // Search for specific layer, then delete that layer
        // Then decrement the insertion iterator count.
        // Removing from actual vector but not actually deleted.
        auto iter = std::find(m_Layers.begin(), m_Layers.end(), layer);

        if(iter != m_Layers.end()){
            m_Layers.erase(iter);
            m_LayersInsertIndex--;
        }
    }

    // Reason why its done that way is because overlays, you always want to render last
    // - If you Push an overlay just at any point, then you want to ensure that it suddenly gets put
    //   at the very end
    // - Whereas layers gets Pushed before the overlays
    //  - meaning it can get Pushed in the last layer, but not in front of overlays. (overlays will always be behind layers)
    void LayerStack::PopOverlay(Layer* overlay){
        auto iter = std::find(m_Layers.begin(), m_Layers.end(), overlay);

        if(iter != m_Layers.end()){
            m_Layers.erase(iter);
        }
    }
};