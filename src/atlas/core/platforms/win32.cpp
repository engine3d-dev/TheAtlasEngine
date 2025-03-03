#include "update_handlers/global_update.hpp"
#include <core/application.hpp>
#include <core/system_framework/system_registry.hpp>

extern atlas::ref<atlas::application> atlas::initialize_application();

int
Main() {
    //! @note DONT FORGET, global_updateManager should not be instantiated here.
    //! @note By doing this it would be reliant when windows initialized/
    //! @note I moved this to be part of the pre-init phase because every other
    //! code relies this to get it working.
    // atlas::global_updateManager* s_global_updateManager =
    // atlas::global_update::GetInstance();
    atlas::ref<atlas::application> app = atlas::initialize_application();
    atlas::system_registry::initialize();
    app->execute();
    atlas::global_update::cleanup();
    return 0;
}
