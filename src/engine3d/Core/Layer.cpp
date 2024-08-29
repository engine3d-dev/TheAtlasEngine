#include <Core/Layer.h>


namespace engine3d{
    static std::string g_LayerName = "Default Name String";
    Layer::Layer(const std::string& name) {
        g_LayerName = name;
    }

    void Layer::OnAttach(){
        BeginPlay();
    }

    void Layer::OnDetach(){
        EndPlay();
    }

    void Layer::OnUpdate(float ts){
        UpdateFrame(ts);
    }

    // void Layer::OnEvent(Event& event){
    //     UpdateEvent(event);
    // }

    void Layer::OnUIRender(){
        UpdateUI();
    }

    std::string Layer::GetLayerString() const { return g_LayerName; }

    // std::string Layer::ExtractLayerDebugName() const { return g_LayerName; }
};