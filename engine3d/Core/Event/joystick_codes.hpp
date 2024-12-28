#pragma once
#include <cstdint>

namespace engine3d{
    namespace Joystick{
        typedef enum class KeyCode : uint64_t{
            JOYSTICK_1 = 0,
            
            JOYSTICK_2 =  1,
            
            JOYSTICK_3 =  2,
            
            JOYSTICK_4 =  3,
            
            JOYSTICK_5 =  4,
            
            JOYSTICK_6 =  5,
            
            JOYSTICK_7 =  6,
            
            JOYSTICK_8 =  7,
            
            JOYSTICK_9 =  8,
            
            JOYSTICK_10 =  9,
            
            JOYSTICK_11  = 10,
            
            JOYSTICK_12 =  11,
            
            JOYSTICK_13 =  12,
            
            JOYSTICK_14 =  13,
            
            JOYSTICK_15 =  14,
            
            JOYSTICK_16 =  15,
            
            JOYSTICK_LAST = JOYSTICK_16,
        } JoystickCodes;
    }
};

#define ENGINE_JOY_STICK_1 ::engine3d::Joystick::JoystickCodes::JOYSTICK_1;
#define ENGINE_JOY_STICK_2 ::engine3d::Joystick::JoystickCodes::JOYSTICK_2;
#define ENGINE_JOY_STICK_3 ::engine3d::Joystick::JoystickCodes::JOYSTICK_3;
#define ENGINE_JOY_STICK_4 ::engine3d::Joystick::JoystickCodes::JOYSTICK_4;
#define ENGINE_JOY_STICK_5 ::engine3d::Joystick::JoystickCodes::JOYSTICK_5;
#define ENGINE_JOY_STICK_6 ::engine3d::Joystick::JoystickCodes::JOYSTICK_6;
#define ENGINE_JOY_STICK_7 ::engine3d::Joystick::JoystickCodes::JOYSTICK_7;
#define ENGINE_JOY_STICK_8 ::engine3d::Joystick::JoystickCodes::JOYSTICK_8;
#define ENGINE_JOY_STICK_9 ::engine3d::Joystick::JoystickCodes::JOYSTICK_9;
#define ENGINE_JOY_STICK_10 ::engine3d::Joystick::JoystickCodes::JOYSTICK_10;
#define ENGINE_JOY_STICK_11 ::engine3d::Joystick::JoystickCodes::JOYSTICK_11;
#define ENGINE_JOY_STICK_12 ::engine3d::Joystick::JoystickCodes::JOYSTICK_12;
#define ENGINE_JOY_STICK_13 ::engine3d::Joystick::JoystickCodes::JOYSTICK_13;
#define ENGINE_JOY_STICK_14 ::engine3d::Joystick::JoystickCodes::JOYSTICK_14;
#define ENGINE_JOY_STICK_15 ::engine3d::Joystick::JoystickCodes::JOYSTICK_15;
#define ENGINE_JOY_STICK_16 ::engine3d::Joystick::JoystickCodes::JOYSTICK_16;
#define ENGINE_JOY_STICK_LAST ::engine3d::Joystick::JoystickCodes::JOYSTICK_LAST;