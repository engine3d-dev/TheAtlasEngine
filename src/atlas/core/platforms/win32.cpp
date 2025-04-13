#include <core/application.hpp>

extern atlas::ref<atlas::application> atlas::initialize_application();

int
win_main() {
    atlas::ref<atlas::application> app = atlas::initialize_application();
    app->execute();

    return 0;
}
