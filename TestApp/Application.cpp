#include "Core/EngineLogger.hpp"
#include <engine3d/Core/ApplicationInstance.hpp>
#include "SceneTest/Scenes/Assets/SceneInstances/ShowCaseSceneInstance.hpp"
#include <string>

namespace engine3d{
    class TestbedApplication : public ApplicationInstance{
    public:
        // TestbedApplication() = default;
        TestbedApplication(const std::string& p_DebugName="Show Case") : ApplicationInstance(p_DebugName) {}
    protected:
        //! @note DO NOT REMOVE... DOES NOTHING BECAUSE DO NOT WANT TO BREAK ANYTHING.
        void UpdateThisApplicationInstance() override{
            ConsoleLogInfo("Called Every Frame");
        }
    private:
        ShowCaseSceneInstance m_SceneManager;
    };

    ApplicationInstance* InitializeApplication(){
        printf("getting application\n");
        return new TestbedApplication();
    }
};