#include <core/engine_logger.hpp>
#include <core/update_handlers/sync_update.hpp>
#include <drivers/vulkan/vulkan_context.hpp>
#include <update_handlers/global_update.hpp>


extern int Main();

int
main() {
    //! @note pre-init phase
    atlas::console_log_manager::initialize_logger_manager();
    atlas::global_update::initialize();
    // atlas::sync_update::initialize();
    atlas::vk::vk_context::initialize();

    return Main();
}