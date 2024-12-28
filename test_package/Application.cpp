#include "Application.hpp"

namespace engine3d{
    TestbedApplication::TestbedApplication(const std::string& p_DebugName) : ApplicationInstance(p_DebugName){
    }

    ApplicationInstance* InitializeApplication(){
        return new TestbedApplication();
    }
};