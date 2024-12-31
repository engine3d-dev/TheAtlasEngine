#include <drivers/vulkan/vulkan_context.hpp>
#include <update_handlers/global_update_manager.hpp>
#include <core/engine_logger.hpp>
#include <core/update_handlers/sync_update_manager.hpp>
extern int Main();


int main(){
    //! @note pre-init phase
    engine3d::ConsoleEngineLogger::InitializeConsoleLogger();
    engine3d::GlobalUpdateManager::Initialize();
    engine3d::SyncUpdateManager::InitializeSyncUpdate();
    engine3d::vk::VulkanContext::Initialize();
    
    return Main();
}