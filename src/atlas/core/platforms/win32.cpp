#include <core/application.hpp>

extern atlas::ref<atlas::application> atlas::initialize_application();

int
Main() {
    atlas::ref<atlas::application> app = atlas::initialize_application();
    app->execute();

    return 0;
}
