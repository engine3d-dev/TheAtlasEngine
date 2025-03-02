#pragma once
#include <core/event/key_codes.hpp>
#include <core/event/mouse_codes.hpp>
#include <glm/glm.hpp>
#include <map>
#include <string>

namespace atlas::event {
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

    bool is_key_pressed(KeyCode p_key);

    bool is_key_released(KeyCode p_key);

    bool is_mouse_pressed(MouseCode p_mouse_code);
    bool is_mouse_released(MouseCode p_mouse_code);

    glm::vec2 cursor_position();

    // joystic-specific functions

    bool is_joystic_present(int p_controller_id);

    const char* is_joystick_guid(int p_controller_id);

    float get_joystic_axis(int p_controller_id, int p_button);

    bool is_joystick_button_released(int p_controller_id);

    bool is_joystick_button_pressed(unsigned char p_button);

    bool is_joystick_button_released(unsigned char p_button);

    // specific for listening events
    void update_events();

    void wait_for_events();
}; // namespace atlas::event