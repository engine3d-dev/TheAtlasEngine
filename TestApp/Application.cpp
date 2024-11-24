#include "Core/EngineLogger.hpp"
#include <engine3d/Core/ApplicationInstance.hpp>
#include <Scenes/Assets/WorldInstances/ShowCaseWorldInstance.hpp>
#include <string>

namespace engine3d{
    class TestbedApplication : public ApplicationInstance{
    public:
        // TestbedApplication() = default;
        TestbedApplication(const std::string& p_DebugName="Show Case") : ApplicationInstance(p_DebugName) 
        {
            m_WorldManager = new ShowCaseWorldInstance();
        }
    private:
        ShowCaseWorldInstance* m_WorldManager;
    };

    ApplicationInstance* InitializeApplication(){
        printf("getting application\n");
        return new TestbedApplication();
    }
};