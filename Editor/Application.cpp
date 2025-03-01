#include <core/application_instance.hpp>
#include <core/engine_logger.hpp>
#include "editor_world.hpp"

class application : public atlas::application_instance{
public:
    application(const atlas::application_settings& p_settings) : application_instance(p_settings){
        console_log_fatal("Application::Application(std::string) gets called!");
        m_world = atlas::create_ref<editor_world>("Editor World");
    }

private:
    atlas::ref<editor_world> m_world;
};

namespace atlas{
    ref<application_instance> initialize_application(){
        //! @note Settings to initiate our application
        //! @note Settings that can be used to set specific properties of the application
        application_settings settings = {
            .Name = "Editor",
            .Width = 1200,
            .Height = 800
            // .Width = 1800,
            // .Height = 1200
        };
        return create_ref<application>(settings);
    }
}