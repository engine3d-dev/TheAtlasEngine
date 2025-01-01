#include <drivers/vulkan/vulkan_context.hpp>
#include <update_handlers/global_update.hpp>
#include <core/engine_logger.hpp>
#include <core/update_handlers/sync_update.hpp>
extern int Main();


int main(){
    //! @note pre-init phase
    engine3d::ConsoleEngineLogger::InitializeConsoleLogger();
    engine3d::GlobalUpdate::Initialize();
    engine3d::SyncUpdate::InitializeSyncUpdate();
    engine3d::vk::VulkanContext::Initialize();
    
    return Main();
}