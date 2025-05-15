#include <core/application.hpp>
#include <core/system_framework/system_registry.hpp>
#include <drivers/graphics_context.hpp>
extern atlas::ref<atlas::application> atlas::initialize_application();

int
win_main() {
    // atlas::system_registry::initialize();
	atlas::ref<atlas::graphics_context> context = atlas::initialize_context();
    atlas::ref<atlas::system_registry> system =
      atlas::create_ref<atlas::system_registry>("system");
    atlas::ref<atlas::application> app = atlas::initialize_application();
    app->execute();

	context->destroy();

    return 0;
}
