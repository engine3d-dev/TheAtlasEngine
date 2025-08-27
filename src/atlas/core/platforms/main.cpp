#include <core/engine_logger.hpp>
#include <GLFW/glfw3.h>
extern int win_main();

int
main() {
    //! @note pre-init phase
    atlas::console_log_manager::initialize_logger_manager();
    //! @note Ensures that glfw gets initiated during the pre-init phases
    if (!glfwInit()) {
        console_log_warn("GLFW: Failed to initialize");
        return -1;
    }

    return win_main();
}