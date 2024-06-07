#include <engine3d/engine3dPrecompiledHeader.h>
#include <engine3d/Event/InputPoll.h>


namespace Engine3D{
    bool InputPoll::IsKeyPressed(KeyCode keycode){
        auto window = static_cast<GLFWwindow *>(Application::Get().GetNativeWindow());

        auto state = glfwGetKey(window, static_cast<int32_t>(keycode));
        return state == GLFW_PRESS || state == GLFW_REPEAT;
    }

    bool InputPoll::IsMousePressed(MouseCode mouseCode){
        auto window = static_cast<GLFWwindow *>(Application::GetNativeWindow());
        auto state = glfwGetMouseButton(window, static_cast<int32_t>(mouseCode));
        return state == GLFW_PRESS;
    }

    glm::vec2 InputPoll::GetMousePos(){
        auto window = static_cast<GLFWwindow *>(Application::GetNativeWindow());
        double xPos, yPos;
        glfwGetCursorPos(window, &xPos, &yPos);

        return {xPos, yPos};
    }

    float InputPoll::GetMouseX(){ return GetMousePos().x; }

    float InputPoll::GetMouseY(){ return GetMousePos().y; }
};