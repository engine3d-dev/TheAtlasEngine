#include <Core/EngineLogger.hpp>
#include <internal/VulkanCpp/Vulkan.hpp>
#include <internal/VulkanCpp/VulkanDevice.hpp>
#include <Core/TimeManagement/GlobalUpdateManager.hpp>
#include <Core/Window.hpp>
#include <Core/Event/InputPoll.hpp>

#include <Core/ApplicationInstance.hpp>

extern engine3d::ApplicationInstance* engine3d::InitializeApplication();

int Main(int argc, char** argv){
    engine3d::ConsoleEngineLogger::InitializeConsoleLogger("engine3d");
    engine3d::vk::Vulkan::InitializeVulkanCore();
    engine3d::vk::VulkanDevice::InitializeVulkanDevice();
    engine3d::GlobalUpdateManager *g_UpdateManager = new engine3d::GlobalUpdateManager();
    engine3d::ApplicationInstance* app = engine3d::InitializeApplication();
    auto& w = engine3d::ApplicationInstance::GetWindow();
    
    while(w.IsActive()){
            // FrameTimer::UpdateFrameTimer(); // give us the frames in flight.

            // Renderer::Presentation();

            //UpdateCurrentApplicationInstance();
            w.OnUpdateAllFrames(g_UpdateManager);

          
            engine3d::InputPoll::UpdateEvents();
        }

        delete g_UpdateManager;

    return 0;
}
