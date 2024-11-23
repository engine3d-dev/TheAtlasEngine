#include "TimeManagement/GlobalUpdateManager.hpp"
#include <Core/EngineLogger.hpp>
// #include <internal/VulkanCpp/Vulkan.hpp>
// #include <internal/VulkanCpp/VulkanDevice.hpp>

#include <Core/ApplicationInstance.hpp>

extern engine3d::ApplicationInstance* engine3d::InitializeApplication();

int Main(int argc, char** argv){
    engine3d::ConsoleEngineLogger::InitializeConsoleLogger("engine3d");
    //! @note DONT FORGET, GlobalUpdateManager should not be instantiated here.
    //! @note By doing this it would be reliant when windows initialized/
    //! @note WILL BE MOVED TO InitializeCore()!!!!
    // engine3d::GlobalUpdateManager* s_GlobalUpdateManager = engine3d::GlobalUpdateManager::GetInstance();

    // engine3d::vk::Vulkan::InitializeVulkanCore();
    // engine3d::vk::VulkanDevice::InitializeVulkanDevice();

    //! @note For now, GlobalUpdateManager will be initialized in InitializeApplication.
    //! @note In the future we add GlobalUpdateManager into InitializeCore() so that
    //! @note its initializations not bound to if we were to have multiple applications engine3d::InitializeCore();

    engine3d::ApplicationInstance* app = engine3d::InitializeApplication();
    app->ExecuteApplicationMainloop();
    return 0;
}
