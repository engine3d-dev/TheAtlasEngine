#pragma once
#include <cstdint>

namespace atlas {
    enum MouseCode : int {
        // From glfw3.h
        Button0 = 0,
        Button1 = 1,
        Button2 = 2,
        Button3 = 3,
        Button4 = 4,
        Button5 = 5,
        Button6 = 6,
        Button7 = 7,

        ButtonLast = Button7,
        ButtonLeft = Button0,
        ButtonRight = Button1,
        ButtonMiddle = Button2
    };

} // namespace atlas

inline constexpr int MOUSE_BUTTON_0 = ::atlas::MouseCode::Button0;
inline constexpr int MOUSE_BUTTON_1 = ::atlas::MouseCode::Button1;
inline constexpr int MOUSE_BUTTON_2 = ::atlas::MouseCode::Button2;
inline constexpr int MOUSE_BUTTON_3 = ::atlas::MouseCode::Button3;
inline constexpr int MOUSE_BUTTON_4 = ::atlas::MouseCode::Button4;
inline constexpr int MOUSE_BUTTON_5 = ::atlas::MouseCode::Button5;
inline constexpr int MOUSE_BUTTON_6 = ::atlas::MouseCode::Button6;
inline constexpr int MOUSE_BUTTON_7 = ::atlas::MouseCode::Button7;
inline constexpr int MOUSE_BUTTON_LAST = ::atlas::MouseCode::ButtonLast;
inline constexpr int MOUSE_BUTTON_LEFT = ::atlas::MouseCode::ButtonLeft;
inline constexpr int MOUSE_BUTTON_RIGHT = ::atlas::MouseCode::ButtonRight;
inline constexpr int MOUSE_BUTTON_MIDDLE = ::atlas::MouseCode::ButtonMiddle;