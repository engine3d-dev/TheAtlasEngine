
#include <Core/EngineLogger.h>
#include <Core/backend/Vulkan/Vulkan.h>
#include <Core/backend/Vulkan/VulkanDevice.h>
#include <Core/backend/Vulkan/VulkanSwapchain.h>
#include <Core/UI/UICore.h>

namespace engine3d{
    //! @note TODO -- Move this somewhere within the vulkan directory
    //! @note Should only be used to initialize Vulkan's API and it's devices (physical and logical)
    void InitializeVulkanCore(){
        //! @note Initializes core vulkan API
        vk::VulkanPipeline::InitializePipeline(); // initializes our vulakn API
        vk::VulkanDevice::InitializeDevice(); // initializes our physical/logical devices
        vk::VulkanSwapchain::InitializeSwaphchain();
        // UICore::InitializeImgui();
    }
};

/**
 * @name Engine3DInitializeCore()
 * @note Initiating Engine3D Core modules
*/
void Engine3DInitializeCore(){
    engine3d::ConsoleEngineLogger::InitializeConsoleLogger("engine3d");
    engine3d::InitializeVulkanCore();
}