#include <Core/backend/Vulkan/Vulkan.h>
#include <GLFW/glfw3.h>
#include <engine3d/Core/Event/InputPoll.h>


namespace engine3d{
    bool InputPoll::IsKeyPressed(KeyCode keycode){
        auto window = VulkanPipeline::GetCurrentWindow();

        auto state = glfwGetKey(window, static_cast<int32_t>(keycode));
        return state == GLFW_PRESS || state == GLFW_REPEAT;
    }

    bool InputPoll::IsMousePressed(MouseCode mouseCode){
        auto window = VulkanPipeline::GetCurrentWindow();
        auto state = glfwGetMouseButton(window, static_cast<int32_t>(mouseCode));
        return state == GLFW_PRESS;
    }

    glm::vec2 InputPoll::GetMousePos(){
        auto window = VulkanPipeline::GetCurrentWindow();
        double xPos, yPos;
        glfwGetCursorPos(window, &xPos, &yPos);

        return {xPos, yPos};
    }

    float InputPoll::GetMouseX(){ return GetMousePos().x; }

    float InputPoll::GetMouseY(){ return GetMousePos().y; }
};