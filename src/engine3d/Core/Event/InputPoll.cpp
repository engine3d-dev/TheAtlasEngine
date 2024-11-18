#include <Core/ApplicationInstance.hpp>
#include <Core/internal/Vulkan2Showcase/Vulkan.hpp>
#include <GLFW/glfw3.h>
#include <Core/Event/InputPoll.hpp>


namespace engine3d{
    bool InputPoll::IsKeyPressed(KeyCode keycode){
        // auto window = ApplicationInstance::Get().GetCurrentWindow();
        auto window = ApplicationInstance::GetWindow().GetNativeWindow();

        auto state = glfwGetKey(window, static_cast<int32_t>(keycode));
        return state == GLFW_PRESS || state == GLFW_REPEAT;
    }

    bool InputPoll::IsMousePressed(MouseCode mouseCode){
        auto window = ApplicationInstance::GetWindow().GetNativeWindow();
        auto state = glfwGetMouseButton(window, static_cast<int32_t>(mouseCode));
        return state == GLFW_PRESS;
    }

    glm::vec2 InputPoll::GetMousePosition(){
        auto window = ApplicationInstance::GetWindow().GetNativeWindow();
        double xPos, yPos;
        glfwGetCursorPos(window, &xPos, &yPos);

        return {xPos, yPos};
    }

    float InputPoll::GetMouseX(){
        return GetMousePosition().x;
    }

    float InputPoll::GetMouseY(){
        return GetMousePosition().y;
    }

    void InputPoll::UpdateEvents(){
        glfwPollEvents();
    }
};