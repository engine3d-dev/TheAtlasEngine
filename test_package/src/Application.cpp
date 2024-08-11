#include <Editor/Editor/Editor.h>

extern void Engine3DInitializeCore();

int main(int argc, char** argv){
    Engine3DInitializeCore();
    ConsoleLogInfo("Test_Package initiated!");
    printf("Testing!\n");
}