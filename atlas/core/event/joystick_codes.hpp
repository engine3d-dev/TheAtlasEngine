#pragma once
#include <cstdint>

namespace atlas {
    namespace Joystick {
        enum JoystickCodes : int {
            JOYSTICK_1 = 0,

            JOYSTICK_2 = 1,

            JOYSTICK_3 = 2,

            JOYSTICK_4 = 3,

            JOYSTICK_5 = 4,

            JOYSTICK_6 = 5,

            JOYSTICK_7 = 6,

            JOYSTICK_8 = 7,

            JOYSTICK_9 = 8,

            JOYSTICK_10 = 9,

            JOYSTICK_11 = 10,

            JOYSTICK_12 = 11,

            JOYSTICK_13 = 12,

            JOYSTICK_14 = 13,

            JOYSTICK_15 = 14,

            JOYSTICK_16 = 15,

            JOYSTICK_LAST = JOYSTICK_16,
        };
    }
}; // namespace atlas

inline constexpr int JOY_STICK_1 = ::atlas::Joystick::JoystickCodes::JOYSTICK_1;
inline constexpr int JOY_STICK_2 = ::atlas::Joystick::JoystickCodes::JOYSTICK_2;
inline constexpr int JOY_STICK_3 = ::atlas::Joystick::JoystickCodes::JOYSTICK_3;
inline constexpr int JOY_STICK_4 = ::atlas::Joystick::JoystickCodes::JOYSTICK_4;
inline constexpr int JOY_STICK_5 = ::atlas::Joystick::JoystickCodes::JOYSTICK_5;
inline constexpr int JOY_STICK_6 = ::atlas::Joystick::JoystickCodes::JOYSTICK_6;
inline constexpr int JOY_STICK_7 = ::atlas::Joystick::JoystickCodes::JOYSTICK_7;
inline constexpr int JOY_STICK_8 = ::atlas::Joystick::JoystickCodes::JOYSTICK_8;
inline constexpr int JOY_STICK_9 = ::atlas::Joystick::JoystickCodes::JOYSTICK_9;
inline constexpr int JOY_STICK_10 =
  ::atlas::Joystick::JoystickCodes::JOYSTICK_10;
inline constexpr int JOY_STICK_11 =
  ::atlas::Joystick::JoystickCodes::JOYSTICK_11;
inline constexpr int JOY_STICK_12 =
  ::atlas::Joystick::JoystickCodes::JOYSTICK_12;
inline constexpr int JOY_STICK_13 =
  ::atlas::Joystick::JoystickCodes::JOYSTICK_13;
inline constexpr int JOY_STICK_14 =
  ::atlas::Joystick::JoystickCodes::JOYSTICK_14;
inline constexpr int JOY_STICK_15 =
  ::atlas::Joystick::JoystickCodes::JOYSTICK_15;
inline constexpr int JOY_STICK_16 =
  ::atlas::Joystick::JoystickCodes::JOYSTICK_16;
inline constexpr int JOY_STICK_LAST =
  ::atlas::Joystick::JoystickCodes::JOYSTICK_LAST;