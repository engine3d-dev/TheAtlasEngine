#include <core/application.hpp>
#include <core/system/registry.hpp>
#include <drivers/graphics_context.hpp>
#include <core/engine_logger.hpp>
extern atlas::ref<atlas::application> atlas::initialize_application();

int
win_main() {
    // atlas::system_registry::initialize();
    atlas::ref<atlas::graphics_context> graphic_context =
      atlas::initialize_context("vulkan_context");

    atlas::ref<atlas::system_registry> system =
      atlas::create_ref<atlas::system_registry>("system");
    atlas::ref<atlas::application> app = atlas::initialize_application();
    app->execute();
    graphic_context->destroy();
    app->post_destroy();
    return 0;
}
