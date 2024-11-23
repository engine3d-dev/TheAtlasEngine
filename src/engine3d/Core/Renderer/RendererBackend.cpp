#include <Core./Renderer/RendererBackend.hpp>
#include <Core/ApplicationInstance.hpp>
#include <internal/Vulkan2Showcase/VulkanRenderer.hpp>

namespace engine3d{

    Ref<RendererBackend> RendererBackend::InitializeRenderer(const std::string& p_DebugName){
        switch (ApplicationInstance::CurrentAPI()){
        case API::VULKAN: return CreateRef<vk::VulkanRenderer>(p_DebugName);
        default: return nullptr;
        }

        return nullptr;
    }

    void RendererBackend::BeginFrame(){
        return StartFrame();
    }

    void RendererBackend::EndFrame(){
        return FinishFrame();
    }
};