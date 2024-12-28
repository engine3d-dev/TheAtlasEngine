#include <core/application_instance.hpp>
#include <core/engine_logger.hpp>
#include <drivers/renderer_backend.hpp>
#include <drivers/vulkan/vulkan_renderer.hpp>

namespace engine3d{
    Scope<RendererContext> RendererContext::Initialize(const std::string &p_Tag){
        switch (ApplicationInstance::CurrentAPI()){
        case API::VULKAN:
            return CreateScope<vk::VulkanRenderer>(p_Tag);
        
        default:
            ConsoleLogFatal("API specified was not supported!");
            return nullptr;
        }
        return nullptr;
    }

    void RendererContext::Begin(){
        return BeginFrame();
    }

    void RendererContext::End(){
        return EndFrame();
    }
};