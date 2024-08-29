/**
 * @name platforms entry points
 * @note Platform entry points target the specific platforms we will want to support
 * @note Windows, Mac, Linux
*/

//! @note Main will be called based on our current platform
extern int Main(int argc, char** argv);


int main(int argc, char** argv){
    return Main(argc, argv);
}