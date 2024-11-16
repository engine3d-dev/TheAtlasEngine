#include <Core/EngineLogger.hpp>
// #include <internal/VulkanCpp/Vulkan.hpp>
// #include <internal/VulkanCpp/VulkanDevice.hpp>

#include <Core/ApplicationInstance.hpp>

extern engine3d::ApplicationInstance* engine3d::InitializeApplication();

int Main(int argc, char** argv){
    engine3d::ConsoleEngineLogger::InitializeConsoleLogger("engine3d");
    // engine3d::vk::Vulkan::InitializeVulkanCore();
    // engine3d::vk::VulkanDevice::InitializeVulkanDevice();

    engine3d::ApplicationInstance* app = engine3d::InitializeApplication();
    app->ExecuteApplicationMainloop();
    return 0;
}
