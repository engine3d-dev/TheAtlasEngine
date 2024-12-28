#include <drivers/vulkan/vulkan_context.hpp>
#include <update_handlers/global_update_manager.hpp>
#include <core/engine_logger.hpp>
#include <core/update_handlers/sync_update_manager.hpp>
extern int Main(int argc, char** argv);


int main(int argc, char** argv){
    //! @note pre-init phase
    engine3d::ConsoleEngineLogger::InitializeConsoleLogger();
    engine3d::SyncUpdateManager::InitializeSyncUpdate();
    engine3d::GlobalUpdateManager::Initialize();
    engine3d::vk::VulkanContext::Initialize();
    
    return Main(argc, argv);
}