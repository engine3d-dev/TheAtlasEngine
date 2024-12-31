// #include "update_handlers/sync_update_manager.hpp"
// #include <core/engine_logger.hpp>
#include "update_handlers/global_update_manager.hpp"
#include <core/application_instance.hpp>

extern engine3d::Ref<engine3d::ApplicationInstance> engine3d::Initialize();

int Main(){
    //! @note DONT FORGET, GlobalUpdateManager should not be instantiated here.
    //! @note By doing this it would be reliant when windows initialized/
    //! @note I moved this to be part of the pre-init phase because every other code relies this to get it working.
    // engine3d::GlobalUpdateManager* s_GlobalUpdateManager = engine3d::GlobalUpdateManager::GetInstance();
    engine3d::Ref<engine3d::ApplicationInstance> app = engine3d::Initialize();
    app->ExecuteMainloop();
    engine3d::GlobalUpdateManager::GlobalCleanup();
    return 0;
}
