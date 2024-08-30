#include <engine3d/Core/ApplicationInstance.h>
#include <string>

namespace engine3d{
    class TestbedApplication : public ApplicationInstance{
    public:
        TestbedApplication(const std::string& p_DebugName);
    protected:
        void UpdateThisApplicationInstance() override;
    };
};

