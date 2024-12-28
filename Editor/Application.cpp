#include "core/event/input_poll.hpp"
#include <core/update_handlers/sync_update_manager.hpp>
#include <core/engine_logger.hpp>
#include <core/update_handlers/global_update_manager.hpp>
#include <core/application_instance.hpp>

namespace engine3d{
    class Application : public engine3d::ApplicationInstance{
    public:
        Application(const std::string& p_Tag) : engine3d::ApplicationInstance(p_Tag){
            SyncUpdateManager::Subscribe(this, &Application::OnUpdate);

            ConsoleLogInfoWithTag("Editor", "Application Initialized Completed!");
            // GlobalUpdateManager::SubscribeApplicationUpdate(this, &Application::OnApplicationUpdate);
        }

        void OnUpdate(){
            // ConsoleLogInfoWithTag("Editor", "OnUpdate called from application!");
            if(InputPoll::IsKeyPressed(ENGINE_KEY_ESCAPE)){
                ApplicationInstance::ShutdownApplication();
            }
        }


        void OnApplicationUpdate(){
            // ConsoleLogInfo("OnApplicationUpdate called from application!");
            ConsoleLogInfoWithTag("Editor", "OnApplicationUpdate called from application!");
        }
    };

    Ref<ApplicationInstance> Initialize(){
        return CreateRef<Application>("Editor");
    }
};