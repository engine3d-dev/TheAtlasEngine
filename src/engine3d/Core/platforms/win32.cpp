
#include <Core/ApplicationInstance.h>
#include "Core/EngineLogger.h"

extern void Engine3DInitializeCore();
extern engine3d::ApplicationInstance* engine3d::InitializeApplication();

/**
 * @param Question
 * @note Platforms are a way that we can target the different platforms developers may want to develop games for.
*/
int Main(int argc, char** argv){
    Engine3DInitializeCore();
    ConsoleLogInfo("Win32 Platform called!");
    engine3d::ApplicationInstance* app = engine3d::InitializeApplication();

    app->StartApplicationRunMainloop();
    // engine3d::Engine3DInitializeApplication();
    // engine3d::ApplicationInstance* app = new engine3d::EditorApplication();
    // app->StartApplicationRunMainloop();
    // engine3d::ApplicationInstance* editor = new engine3d::ApplicationInstance();
    // editor->StartApplicationRunMainloop();
    return 0;
}