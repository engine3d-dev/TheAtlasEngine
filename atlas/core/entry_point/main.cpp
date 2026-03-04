// For glfwInit call
#include <GLFW/glfw3.h>
import atlas.application;
import atlas.common;
import atlas.logger;
import atlas.graphics_api;
import atlas.drivers.graphics_context;
import atlas.renderer.context_loader;

// Defined in the user-application side
[[nodiscard]] atlas::ref<atlas::application> initialize_application(
  atlas::ref<atlas::graphics_context> p_context);

int
main() {
    atlas::console_log_manager manager = atlas::console_log_manager();

    if (!glfwInit()) {
        console_log_fatal("GLFW: Initialization failed!!");
        return -1;
    }

    atlas::ref<atlas::graphics_context> context =
      atlas::initialize_context("vulkan", atlas::graphics_api::vulkan);

    atlas::ref<atlas::application> app = initialize_application(context);

    app->execute();

    app->post_destroy();

    context->destroy();
    return 0;
}