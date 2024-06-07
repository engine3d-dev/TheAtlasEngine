#include <engine3d/engine3dPrecompiledHeader.h>
#include <engine3d/Core/EntryPoint.h>

namespace Engine3D{
    extern Application* CreateApplication();
#if _WIN32
    int Main(int argc, char** argv){
        EngineLogger::Initialize();
        auto app = CreateApplication();
        app->Run();
        delete app;
        return 0;
    }
#else
    int Main(int argc, char** argv){
        EngineLogger::Initialize();
        auto app = CreateApplication();
        app->Run();
        delete app;
        return 0;
    }
#endif
};

int main(int argc, char** argv){
    return Engine3D::Main(argc, argv);
}
