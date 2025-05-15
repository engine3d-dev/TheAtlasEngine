#include <core/engine_logger.hpp>

extern int win_main();

int
main() {
    //! @note pre-init phase
    atlas::console_log_manager::initialize_logger_manager();

    // atlas::global_update::initialize();
    // atlas::sync_update::initialize();
    // atlas::vk::vk_context::initialize();

    return win_main();
}