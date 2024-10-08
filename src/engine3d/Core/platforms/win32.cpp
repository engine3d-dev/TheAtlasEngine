#include "Core/EngineLogger.h"
#include <internal/VulkanCpp/Vulkan.h>
#include <internal/VulkanCpp/VulkanDevice.h>

#include <Core/ApplicationInstance.h>

extern engine3d::ApplicationInstance* engine3d::InitializeApplication();

int Main(int argc, char** argv){
    engine3d::ConsoleEngineLogger::InitializeConsoleLogger("engine3d");
    engine3d::vk::Vulkan::InitializeVulkanCore();
    engine3d::vk::VulkanDevice::InitializeVulkanDevice();

    engine3d::ApplicationInstance* app = engine3d::InitializeApplication();
    app->ExecuteApplicationMainloop();
    return 0;
}