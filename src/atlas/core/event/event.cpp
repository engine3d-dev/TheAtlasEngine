#include <GLFW/glfw3.h>
#include <core/event/event.hpp>
#include <core/application_instance.hpp>

namespace atlas::event{
    static std::map<uint32_t, joystick_info> s_controllers;

    bool is_key_pressed(KeyCode p_key){
        GLFWwindow* window = application_instance::get_window();

        auto state = glfwGetKey(window, static_cast<int32_t>(p_key));
        return (state == GLFW_PRESS);
    }

    bool is_key_released(KeyCode p_key){
        GLFWwindow* window = application_instance::get_window();

        auto state = glfwGetKey(window, static_cast<int32_t>(p_key));
        return (state == GLFW_RELEASE);
    }

    bool is_mouse_pressed(MouseCode p_mouse_code){
        GLFWwindow* window = application_instance::get_window();

        auto state = glfwGetMouseButton(window, static_cast<int32_t>(p_mouse_code));
        return (state == GLFW_PRESS);
    }

    bool is_mouse_released(MouseCode p_mouse_code){
        GLFWwindow* window = application_instance::get_window();

        auto state = glfwGetMouseButton(window, static_cast<int32_t>(p_mouse_code));
        return (state == GLFW_RELEASE);
    }

    glm::vec2 cursor_position(){
        GLFWwindow* window = application_instance::get_window();

        double xPos, yPos;
        glfwGetCursorPos(window, &xPos, &yPos);

        return {xPos, yPos};
    }


    // joystic-specific functions

    bool is_joystic_present(int p_controller_id){
        return s_controllers.contains(p_controller_id);
    }

    const char* is_joystick_guid(int p_controller_id){
        return glfwGetJoystickGUID(p_controller_id);
    }

    float get_joystic_axis(int p_controller_id, int p_button){
        int count;
        const float axes = glfwGetJoystickAxes(p_controller_id, &count)[p_button];
        
        if(count < p_button){
            return 0.0f;
        }
        else{
            return axes;
        }
    }

    bool is_joystick_button_released(unsigned char p_button){
        return (p_button == GLFW_PRESS);
    }

    bool is_joystick_button_pressed(unsigned char p_button){
        return (p_button == GLFW_RELEASE);
    }

    // specific for listening events
    void update_events(){
        glfwPollEvents();

        // updating joysticks here
        // ....
        //! @note Must be called per input updated events. In the case either game console disconnects or reconnects
        //! @note This will continously check.
        //! @note By default GLFW check's up to a total of 16 joystick ID's that are available
        //! @note We iterate all 16 joysticks, only using the joystic ID's that are connected
        //! @note Then checking for any events from the connected joystick has occurred
        // 1 is the first joystick.
        // 16 is the last joystick
        for(int joystick_id = 0; joystick_id < 16; joystick_id++){
            if(glfwJoystickPresent(joystick_id) == GLFW_TRUE){
                auto& joystick = s_controllers[joystick_id];
                joystick.ID = joystick_id;
                joystick.JoystickName = glfwGetJoystickName(joystick_id);

                //! @note We always check how many buttons the joysticks that are connected contain.
                int amount_of_buttons = -1;
                const unsigned char* buttons = glfwGetJoystickButtons(joystick_id, &amount_of_buttons);

                // ConsoleLogWarn("Button Size = {}", amount_of_buttons);

                for(int i = 0; i < amount_of_buttons; i++){
                    // ConsoleLogFatal("Button {} is ===> {}", i, buttons[i]);
                    // if(buttons[i] == GLFW_PRESS && !joystick.ButtonsDown[i]){
                    if(is_joystick_button_pressed(buttons[i]) && !joystick.ButtonsDown[i]){
                        joystick.Buttons[i].ButtonState = input_state::Pressed;
                    }
                    // else if(buttons[i] == GLFW_RELEASE and joystick.ButtonsDown[i]){
                    else if(is_joystick_button_released(buttons[i]) and joystick.ButtonsDown[i]){
                        joystick.Buttons[i].ButtonState = input_state::Released;
                    }

                    // joystick.ButtonsDown[i] = (buttons[i] == GLFW_PRESS);
                    joystick.ButtonsDown[i] = is_joystick_button_pressed(buttons[i]);
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

    void wait_for_events(){}
};