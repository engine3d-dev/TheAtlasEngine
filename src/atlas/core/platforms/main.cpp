#include <core/engine_logger.hpp>
#include <drivers/vulkan/vulkan_context.hpp>
#include <core/system_framework/system_registry.hpp>

extern int Main();

int
main() {
    //! @note pre-init phase
    atlas::console_log_manager::initialize_logger_manager();

    // atlas::global_update::initialize();
    // atlas::sync_update::initialize();
    atlas::vk::vk_context::initialize();
    atlas::system_registry::initialize();

    return Main();
}