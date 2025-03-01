#pragma once
#include <cstdint>

namespace atlas{
    namespace Joystick{
        enum class JoystickCodes : uint64_t{
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
        };
    }
};

#define JOY_STICK_1 ::atlas::Joystick::JOYSTICK_1;
#define JOY_STICK_2 ::atlas::Joystick::JOYSTICK_2;
#define JOY_STICK_3 ::atlas::Joystick::JOYSTICK_3;
#define JOY_STICK_4 ::atlas::Joystick::JOYSTICK_4;
#define JOY_STICK_5 ::atlas::Joystick::JOYSTICK_5;
#define JOY_STICK_6 ::atlas::Joystick::JOYSTICK_6;
#define JOY_STICK_7 ::atlas::Joystick::JOYSTICK_7;
#define JOY_STICK_8 ::atlas::Joystick::JOYSTICK_8;
#define JOY_STICK_9 ::atlas::Joystick::JOYSTICK_9;
#define JOY_STICK_10 ::atlas::Joystick::JOYSTICK_10;
#define JOY_STICK_11 ::atlas::Joystick::JOYSTICK_11;
#define JOY_STICK_12 ::atlas::Joystick::JOYSTICK_12;
#define JOY_STICK_13 ::atlas::Joystick::JOYSTICK_13;
#define JOY_STICK_14 ::atlas::Joystick::JOYSTICK_14;
#define JOY_STICK_15 ::atlas::Joystick::JOYSTICK_15;
#define JOY_STICK_16 ::atlas::Joystick::JOYSTICK_16;
#define JOY_STICK_LAST ::atlas::Joystick::JOYSTICK_LAST;