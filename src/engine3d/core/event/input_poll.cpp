#include <core/application_instance.hpp>
#include <GLFW/glfw3.h>
#include <core/event/input_poll.hpp>
#include <core/event/joystick_codes.hpp>

namespace engine3d{
    std::map<int, InputPoll::ControllerProperties> InputPoll::s_Controllers;

    bool InputPoll::IsKeyPressed(KeyCode keycode){
        auto window = ApplicationInstance::GetWindow().GetNativeWindow();

        auto state = glfwGetKey(window, static_cast<int32_t>(keycode));
        return state == GLFW_PRESS || state == GLFW_REPEAT;
    }

    bool InputPoll::IsKeyReleased(KeyCode keyCode){
        auto window = ApplicationInstance::GetWindow().GetNativeWindow();

        auto state = glfwGetKey(window, static_cast<int32_t>(keyCode));
        return state == GLFW_RELEASE;
    }

    bool InputPoll::IsMousePressed(MouseCode mouseCode){
        auto window = ApplicationInstance::GetWindow().GetNativeWindow();
        auto state = glfwGetMouseButton(window, static_cast<int32_t>(mouseCode));
        return state == GLFW_PRESS;
    }

    bool InputPoll::IsMouseReleased(MouseCode mouseCode){
        auto window = ApplicationInstance::GetWindow().GetNativeWindow();
        auto state = glfwGetMouseButton(window, static_cast<int32_t>(mouseCode));
        return state == GLFW_RELEASE;
    }

    void InputPoll::WaitForEvents(){
        glfwWaitEvents();
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

    bool InputPoll::IsControllerPresent(int p_ControllerID){
        return s_Controllers.contains(p_ControllerID);
    }

    // bool InputPoll::IsControllerButtonPressed(int p_ControllerID, int p_Button){
    //     return false;
    // }

    bool InputPoll::IsControllerButtonReleased(int p_ControllerID, int p_Button){
        if(!IsControllerPresent(p_ControllerID)){
            return false;
        }

        auto& joystick = s_Controllers[p_ControllerID];
        return joystick.Buttons.find(p_Button) != joystick.Buttons.end() and joystick.Buttons[p_Button].ButtonState == InputState::PRESSED;
    }

    const char* InputPoll::IsControllerGUID(int p_ControllerID){
        return glfwGetJoystickGUID(p_ControllerID);
    }

    //! @note p_ControllerID defines the controller we want to get our axes.
    //! @note p_Index gives us the location of axes that we want.
    float InputPoll::GetControllerAxis(int p_ControllerID, int p_LocationOfAxes){
        int count;
        auto axes = glfwGetJoystickAxes(p_ControllerID, &count)[p_LocationOfAxes];
        
        if(count < p_LocationOfAxes){
            return 0.0f;
        }
        else{
            return axes;
        }
        return 0.0f;
        // return std::clamp(*s_Controllers[p_ControllerID].AxesOfController[p_LocationOfAxes], 0.01f, 1.0f);
    }


    bool InputPoll::IsControllerButtonPressed(unsigned char button){
        return (button == GLFW_PRESS);
    }

    bool InputPoll::IsControllerButtonReleased(unsigned char button){
        return (button == GLFW_RELEASE);
    }

    void InputPoll::Update(){
        glfwPollEvents();

        //! @note Must be called per input updated events. In the case either game console disconnects or reconnects
        //! @note This will continously check.
        //! @note By default GLFW check's up to a total of 16 joystick ID's that are available
        //! @note We iterate all 16 joysticks, only using the joystic ID's that are connected
        //! @note Then checking for any events from the connected joystick has occurred
        // 1 is the first joystick.
        // 16 is the last joystick
        for(int joystick_id = 0; joystick_id < 16; joystick_id++){
            if(glfwJoystickPresent(joystick_id) == GLFW_TRUE){
                auto& joystick = s_Controllers[joystick_id];
                joystick.ID = joystick_id;
                joystick.JoystickName = glfwGetJoystickName(joystick_id);

                //! @note We always check how many buttons the joysticks that are connected contain.
                int amount_of_buttons = -1;
                const unsigned char* buttons = glfwGetJoystickButtons(joystick_id, &amount_of_buttons);

                // ConsoleLogWarn("Button Size = {}", amount_of_buttons);

                for(int i = 0; i < amount_of_buttons; i++){
                    // ConsoleLogFatal("Button {} is ===> {}", i, buttons[i]);
                    // if(buttons[i] == GLFW_PRESS && !joystick.ButtonsDown[i]){
                    if(IsControllerButtonPressed(buttons[i]) && !joystick.ButtonsDown[i]){
                        joystick.Buttons[i].ButtonState = InputState::PRESSED;
                    }
                    // else if(buttons[i] == GLFW_RELEASE and joystick.ButtonsDown[i]){
                    else if(IsControllerButtonReleased(buttons[i]) and joystick.ButtonsDown[i]){
                        joystick.Buttons[i].ButtonState = InputState::RELEASED;
                    }

                    // joystick.ButtonsDown[i] = (buttons[i] == GLFW_PRESS);
                    joystick.ButtonsDown[i] = IsControllerButtonPressed(buttons[i]);
                }

                int amount_of_axes = -1;
                const float* axes = glfwGetJoystickAxes(joystick_id, &amount_of_axes);
                joystick.AxesOfController[joystick_id] = axes;
                // ConsoleLogFatal("Axes at for-loop i = {} and Axes value = {:.3f}", 0, axes[0]);
                // ConsoleLogFatal("Axes at for-loop i = {} and Axes value = {:.3f}", 1, axes[1]);
                // for(int i = 0; i < amount_of_axes; i++){
                //     ConsoleLogFatal("Axes at for-loop i = {} and Axes value = {:.3f}", i, axes[i]);
                // }

            }
        }
    }
};