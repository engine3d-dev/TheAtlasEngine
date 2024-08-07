#include <Editor/Editor/Editor.h>

extern void Engine3DInitializeCore();

/**
 * @param Question
 * @note Platforms are a way that we can target the different platforms developers may want to develop games for.
*/
int Main(int argc, char** argv){
    Engine3DInitializeCore();
    ConsoleLogInfo("Initiating Win32 supported platform");
    engine3d::EditorApplication* editor = new engine3d::EditorApplication();
    editor->RunEditor();
    return 0;
}