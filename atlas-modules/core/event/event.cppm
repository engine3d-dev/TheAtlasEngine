module;

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <string>
#include <map>
#include <cstdint>

export module atlas.core.event;

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
    static GLFWwindow* s_window_address;

    export namespace event {
        void flush_events() {
            glfwPollEvents();
        }

        bool is_key_pressed(int p_key) {

            // GLFWwindow* window = application::get_window();
            GLFWwindow* window = s_window_address;

            auto state = glfwGetKey(window, static_cast<int32_t>(p_key));
            return (state == GLFW_PRESS);
        }

        bool is_key_released(int p_key) {}

        bool is_mouse_pressed(int p_mouse_code) {}
        bool is_mouse_released(int p_mouse_code) {}

        glm::vec2 cursor_position() {}

        // joystic-specific functions
        bool is_joystic_present(int p_controller_id) {}

        const char* is_joystick_guid(int p_controller_id) {}

        float get_joystic_axis(int p_controller_id, int p_button) {}

        // bool is_button_pressed(int p_button_id, int p_controller_id);

        // bool is_button_released(int p_button_id, int p_controller_id);

        //! @note FIXME: Make button later
        bool is_joystick_button_pressed(int p_button) {}

        bool is_joystick_button_released(int p_button) {}

        // specific for listening events
        void update_events() {}

        void wait_for_events() {}

        void set_window_size(GLFWwindow* p_window) {
            s_window_address = p_window;
        }
    };

};