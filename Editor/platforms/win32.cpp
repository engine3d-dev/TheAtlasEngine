#include <Editor/Editor/Editor.h>

int Main(int argc, char** argv){
    engine3d::EditorApplication* editor = new engine3d::EditorApplication();
    CoreLogInfo("Initiating Win32 supported platform");
    editor->RunEditor();
    return 0;
}