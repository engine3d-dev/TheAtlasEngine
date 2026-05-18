// For glfwInit call
#include <GLFW/glfw3.h>
import atlas.application;
import atlas.common;
import atlas.logger;
import atlas.graphics_api;
import atlas.drivers.graphics_context;
import atlas.renderer.context_loader;
import atlas.core.event;

#ifdef ENABLE_TESTS_ONLY
#include <boost/ut.hpp>
import atlas.tests;
#endif

// Defined in the user-application side
#ifndef ENABLE_TESTS_ONLY
[[nodiscard]] atlas::ref<atlas::application> initialize_application(
  atlas::ref<atlas::graphics_context> p_context,
  atlas::event::bus& p_bus);
#endif

int
main() {
#ifndef ENABLE_TESTS_ONLY
    // We should not have our core system start up during testing environment
    // execution
    atlas::console_log_manager manager = atlas::console_log_manager();
    if (!glfwInit()) {
        console_log_fatal("GLFW: Initialization failed!!");
        return -1;
    }

    atlas::event::bus event_bus;
    atlas::ref<atlas::graphics_context> context =
      atlas::initialize_context("vulkan", atlas::graphics_api::vulkan);

    atlas::ref<atlas::application> app =
      initialize_application(context, event_bus);

    app->execute();

    app->post_destroy();

    context->destroy();
#else
    initialize_tests();
#endif
    return 0;
}