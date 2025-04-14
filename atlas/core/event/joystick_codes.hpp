#pragma once
#include <cstdint>

namespace atlas::event {
    enum JoystickCodes : int {
        Joystick1 = 0,

        Joystick2 = 1,

        Joystick3 = 2,

        Joystick4 = 3,

        Joystick5 = 4,

        Joystick6 = 5,

        Joystick7 = 6,

        Joystick8 = 7,

        Joystick9 = 8,

        Joystick10 = 9,

        Joystick11 = 10,

        Joystick12 = 11,

        Joystick13 = 12,

        Joystick14 = 13,

        Joystick15 = 14,

        Joystick16 = 15,

        JoystickLAST = Joystick16,
    };
}; // namespace atlas

inline constexpr int joystick_1 = ::atlas::event::JoystickCodes::Joystick1;
inline constexpr int joystick_2 = ::atlas::event::JoystickCodes::Joystick2;
inline constexpr int joystick_3 = ::atlas::event::JoystickCodes::Joystick3;
inline constexpr int joystick_4 = ::atlas::event::JoystickCodes::Joystick4;
inline constexpr int joystick_5 = ::atlas::event::JoystickCodes::Joystick5;
inline constexpr int joystick_6 = ::atlas::event::JoystickCodes::Joystick6;
inline constexpr int joystick_7 = ::atlas::event::JoystickCodes::Joystick7;
inline constexpr int joystick_8 = ::atlas::event::JoystickCodes::Joystick8;
inline constexpr int joystick_9 = ::atlas::event::JoystickCodes::Joystick9;
inline constexpr int joystick_10 = ::atlas::event::JoystickCodes::Joystick10;
inline constexpr int joystick_11 = ::atlas::event::JoystickCodes::Joystick11;
inline constexpr int joystick_12 = ::atlas::event::JoystickCodes::Joystick12;
inline constexpr int joystick_13 = ::atlas::event::JoystickCodes::Joystick13;
inline constexpr int joystick_14 = ::atlas::event::JoystickCodes::Joystick14;
inline constexpr int joystick_15 = ::atlas::event::JoystickCodes::Joystick15;
inline constexpr int joystick_16 = ::atlas::event::JoystickCodes::Joystick16;
inline constexpr int joystick_last =
  ::atlas::event::JoystickCodes::JoystickLAST;