#include <internal/VulkanCpp/Vulkan.h>
#include <Core/EngineLogger.h>

namespace engine3d{
    //! @note TODO -- Move this somewhere within the vulkan directory
    //! @note Should only be used to initialize Vulkan's API and it's devices (physical and logical)
    void InitializeVulkanCore(){
        //! @note Initializes core vulkan API
        vk::Vulkan::InitializeVulkanCore();
        // vk::VulkanSwapchain::InitializeSwaphchain();
        // UICore::InitializeImgui();
    }
};

/**
 * @name Engine3DInitializeCore()
 * @note Initiating Engine3D Core modules
*/
void Engine3DInitializeCore(){
    printf("Engine3DInitializeCore() Called!\n");
    engine3d::ConsoleEngineLogger::InitializeConsoleLogger("engine3d");
    engine3d::InitializeVulkanCore();
}