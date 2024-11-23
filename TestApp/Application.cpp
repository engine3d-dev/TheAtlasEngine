#include "Core/EngineLogger.hpp"
#include <engine3d/Core/ApplicationInstance.hpp>
#include "Scenes/Assets/SceneInstances/ShowCaseSceneInstance.hpp"
#include <string>

namespace engine3d{
    class TestbedApplication : public ApplicationInstance{
    public:
        // TestbedApplication() = default;
        TestbedApplication(const std::string& p_DebugName="Show Case") : ApplicationInstance(p_DebugName) {}
    private:
        ShowCaseSceneInstance m_SceneManager;
    };

    ApplicationInstance* InitializeApplication(){
        printf("getting application\n");
        return new TestbedApplication();
    }
};