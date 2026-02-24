module;

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <string>
#include <map>
#include <cstdint>

export module atlas.core.event;

export import :keys;
export import :mouse_codes;
export import :types;
export import :listener;
export import :bus;

namespace atlas {

    /**
     * @name event.hpp
     * @note Actual input polling system to poll in differeny sets of key/mouse
     * actions
     * @param UpdateEvents handles making sure that all of our events we handle
     * have been successfully updated.
     * @param GetMousePos just returns the position of our mouse cursor
     */
    enum input_state { None, Idle, Pressed, Released };

    struct joystick_button {
        int ID = -1;
        std::string Name = "";
        input_state ButtonState = input_state::None;
        input_state PreviousButtonState = input_state::None;
    };

    struct joystick_info {
        int ID = -1;
        std::string JoystickName = "Default";
        std::map<int, joystick_button> Buttons;
        std::map<int, bool> ButtonsDown;
        std::map<int, const float*> AxesOfController;
    };

    static std::map<uint32_t, joystick_info> s_controllers;

    using button_id = int;
    using controller_id = int;

    // TODO: Probably make this into std::map<uint32_t, GLFWwindow*>
    // s_window_viewports?
    static GLFWwindow* s_window_address;

    export namespace event {

        bool is_key_pressed(int p_key) {

            // GLFWwindow* window = application::get_window();
            GLFWwindow* window = s_window_address;

            auto state = glfwGetKey(window, static_cast<int32_t>(p_key));
            return (state == GLFW_PRESS);
        }

        bool is_key_released(int p_key) {
            GLFWwindow* window = s_window_address;

            auto state = glfwGetKey(window, static_cast<int32_t>(p_key));
            return (state == GLFW_RELEASE);
        }

        bool is_mouse_pressed(int p_mouse_code) {
            GLFWwindow* window = s_window_address;

            auto state =
              glfwGetMouseButton(window, static_cast<int32_t>(p_mouse_code));
            return (state == GLFW_PRESS);
        }

        bool is_mouse_released(int p_mouse_code) {
            GLFWwindow* window = s_window_address;

            auto state =
              glfwGetMouseButton(window, static_cast<int32_t>(p_mouse_code));
            return (state == GLFW_RELEASE);
        }

        glm::vec2 cursor_position() {
            GLFWwindow* window = s_window_address;

            double x_pos, y_pos;
            glfwGetCursorPos(window, &x_pos, &y_pos);

            return { x_pos, y_pos };
        }

        // joystic-specific functions
        bool is_joystic_present(int p_controller_id) {
            return s_controllers.contains(p_controller_id);
        }

        const char* is_joystick_guid(int p_controller_id) {
            return glfwGetJoystickGUID(p_controller_id);
        }

        float get_joystic_axis(int p_controller_id, int p_button) {
            int count;
            const float axes =
              glfwGetJoystickAxes(p_controller_id, &count)[p_button];

            if (count < p_button) {
                return 0.0f;
            }
            else {
                return axes;
            }
        }

        // bool is_button_pressed(int p_button_id, int p_controller_id) {
        //     auto controller = s_controllers[p_controller_id];
        //     return (controller.Buttons[p_button_id].ButtonState ==
        //     GLFW_RELEASE);
        // }

        // bool is_button_released(button_id p_button, controller_id
        // p_controller) {
        //     auto selected_controller = s_controllers[p_controller];
        //     return (selected_controller.Buttons[p_button].ButtonState ==
        //             input_state::Pressed);
        // }

        //! @note FIXME: Make button later
        bool is_joystick_button_pressed(int p_button) {
            return p_button == GLFW_PRESS;
        }

        bool is_joystick_button_released(int p_button) {
            return p_button == GLFW_RELEASE;
        }

        void wait_for_events() {}

        void set_window_size(GLFWwindow* p_window) {
            s_window_address = p_window;
        }

        // specfying listening for events
        void flush_events() {
            glfwPollEvents();

            // updating joysticks here
            // ....
            //! @note Must be called per input updated events. In the case
            //! either game console disconnects or reconnects
            //! @note This will continously check.
            //! @note By default GLFW check's up to a total of 16 joystick ID's
            //! that are available
            //! @note We iterate all 16 joysticks, only using the joystic ID's
            //! that are connected
            //! @note Then checking for any events from the connected joystick
            //! has occurred
            // 1 is the first joystick.
            // 16 is the last joystick
            for (int joystick_id = 0; joystick_id < 16; joystick_id++) {
                if (glfwJoystickPresent(joystick_id) == GLFW_TRUE) {
                    auto& joystick = s_controllers[joystick_id];
                    joystick.ID = joystick_id;
                    joystick.JoystickName = glfwGetJoystickName(joystick_id);

                    //! @note We always check how many buttons the joysticks
                    //! that are connected contain.
                    int amount_of_buttons = -1;
                    const unsigned char* buttons =
                      glfwGetJoystickButtons(joystick_id, &amount_of_buttons);

                    // ConsoleLogWarn("Button Size = {}", amount_of_buttons);

                    for (int i = 0; i < amount_of_buttons; i++) {
                        // ConsoleLogFatal("Button {} is ===> {}", i,
                        // buttons[i]); if(buttons[i] == GLFW_PRESS &&
                        // !joystick.ButtonsDown[i]){
                        if (is_joystick_button_pressed(buttons[i]) &&
                            !joystick.ButtonsDown[i]) {
                            joystick.Buttons[i].ButtonState =
                              input_state::Pressed;
                        }
                        // else if(buttons[i] == GLFW_RELEASE and
                        // joystick.ButtonsDown[i]){
                        else if (is_joystick_button_released(buttons[i]) and
                                 joystick.ButtonsDown[i]) {
                            joystick.Buttons[i].ButtonState =
                              input_state::Released;
                        }

                        // joystick.ButtonsDown[i] = (buttons[i] == GLFW_PRESS);
                        joystick.ButtonsDown[i] =
                          is_joystick_button_pressed(buttons[i]);
                    }

                    int amount_of_axes = -1;
                    const float* axes =
                      glfwGetJoystickAxes(joystick_id, &amount_of_axes);
                    joystick.AxesOfController[joystick_id] = axes;
                    // ConsoleLogFatal("Axes at for-loop i = {} and Axes value =
                    // {:.3f}", 0, axes[0]); ConsoleLogFatal("Axes at for-loop i
                    // =
                    // {} and Axes value = {:.3f}", 1, axes[1]); for(int i = 0;
                    // i < amount_of_axes; i++){
                    //     ConsoleLogFatal("Axes at for-loop i = {} and Axes
                    //     value =
                    //     {:.3f}", i, axes[i]);
                    // }
                }
                else {
                    if (is_joystic_present(joystick_id)) {
                        s_controllers.erase(joystick_id);
                    }
                }
            }
        }
    };

};