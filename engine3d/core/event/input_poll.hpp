#pragma once
#include <core/event/key_codes.hpp>
#include <core/event/mouse_codes.hpp>
#include <map>
#include <string>
#include <glm/glm.hpp>

namespace engine3d{
    /**
     * @name InputPoll.hpp
     * @note Actual input polling system to poll in differeny sets of key/mouse actions
     * @param UpdateEvents handles making sure that all of our events we handle have been successfully updated.
     * @param GetMousePos just returns the position of our mouse cursor
     */
    class InputPoll{
    public:
        enum InputState{
            NONE, IDLE, PRESSED, RELEASED
        };

        struct ControllerButton{
            int ButtonID = -1;
            std::string Name;
            InputState ButtonState = InputState::NONE;
            InputState PreviousButtonState = InputState::NONE;
        };

        //! @note Defines per Joystick Information
        struct ControllerProperties{
            int ID = -1;
            std::string JoystickName = "Default";
            std::map<int, ControllerButton> Buttons;
            std::map<int, bool> ButtonsDown;
            std::map<int, const float*> AxesOfController;
        };

        InputPoll() = delete;
        InputPoll(const InputPoll&) = delete;
        void operator=(const InputPoll&) = delete;

        //! @note Key/Mouse event pressed!
        static bool IsKeyPressed(KeyCode keycode);

        static bool IsKeyReleased(KeyCode keyCode);

        static bool IsMousePressed(MouseCode mouseCode);

        static bool IsMouseReleased(MouseCode mouseCode);

        //! @note Mouse Position
        static glm::vec2 GetMousePosition();

        static float GetMouseX();

        static float GetMouseY();


        //! @note Controller stuff.

        static bool IsControllerPresent(int p_ControllerID);

        static const char* IsControllerGUID(int p_ControllerID);

        static float GetControllerAxis(int p_Controller, int p_Index);

        static bool IsControllerButtonReleased(int p_ControllerID, int p_Button);

        //! @note Assuring that our events are correctly being updated
        static void Update();

        static void WaitForEvents();

        static bool IsControllerButtonPressed(unsigned char button);

        static bool IsControllerButtonReleased(unsigned char button);

        static std::map<int, ControllerProperties> GetControllers() { return s_Controllers; }
    private:
        static std::map<int, ControllerProperties> s_Controllers;
    };
};