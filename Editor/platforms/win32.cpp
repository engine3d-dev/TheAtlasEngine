#include <Editor/Editor/Editor.h>

extern void Engine3DInitializeCore();

/**
 * @param Question
 * @note Platforms are a way that we can target the different platforms developers may want to develop games for.
*/
int Main(int argc, char** argv){
    Engine3DInitializeCore();
    engine3d::EditorApplication* editor = new engine3d::EditorApplication();
    CoreLogInfo("Initiating Win32 supported platform");
    editor->RunEditor();
    return 0;
}