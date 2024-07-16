#include <Core/backend/utilities/helper_functions.h>
#include <GLFW/glfw3.h>

namespace engine3d{
    //! @param currentWindowCtx
    //! @note Our current window context for passing in our window.
    //! @param isWindowOpen
    //! @note We can't gurantee that the window will be open, and should check for this to prevent errors in centering an unopened window.
    void CenterCurrentWindow(GLFWwindow* currentWindowCtx, int currentWindowWidth, int currentWindowHeight, bool isWindowOpened){

        //! @note If window unopened, we dont center our window at all.
        if(!isWindowOpened){
            return;
        }

        //! @note Getting our current monitor
        GLFWmonitor* monitor = glfwGetPrimaryMonitor();
        //! @note Getting our current window data
        const GLFWvidmode* mode = glfwGetVideoMode(monitor);

        //! @note Based on our offset for window dimensions setting the application by default to the center of the screen.
        //! @note TODO --- Should cache our dimensions and window locs so when users open up engine3d again, it will appear back in it's previous last location when it was opened.
        glfwSetWindowPos(currentWindowCtx, (mode->width - currentWindowWidth) / 2, (mode->height / currentWindowHeight) / 2);
    }
};