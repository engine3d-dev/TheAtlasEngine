#include <engine3d/engine3dPrecompiledHeader.h>
#include <engine3d/Core/Layer.h>

namespace Engine3D{
    Layer::Layer(const std::string& name) : m_DebugName(name){}

    Layer::~Layer(){}

    void Layer::OnAttach(){
        Attach();
    }

    void Layer::OnDetach(){
        Detach();
    }

    void Layer::OnEvent(Event& event){
        UpdateEvent(event);
    }

    void Layer::OnUpdate(Timestep ts){
        UpdateLayer(ts);
    }

    void Layer::OnUIRender(){
        UIRender();
    }

    std::string Layer::GetName() const { return m_DebugName; }
};