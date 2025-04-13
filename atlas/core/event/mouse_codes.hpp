#pragma once
#include <cstdint>

namespace atlas::event {
    enum Mouse : uint32_t {
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

inline constexpr uint32_t mouse_button_0 = ::atlas::event::Mouse::Button0;
inline constexpr uint32_t mouse_button_1 = ::atlas::event::Mouse::Button1;
inline constexpr uint32_t mouse_button_2 = ::atlas::event::Mouse::Button2;
inline constexpr uint32_t mouse_button_3 = ::atlas::event::Mouse::Button3;
inline constexpr uint32_t mouse_button_4 = ::atlas::event::Mouse::Button4;
inline constexpr uint32_t mouse_button_5 = ::atlas::event::Mouse::Button5;
inline constexpr uint32_t mouse_button_6 = ::atlas::event::Mouse::Button6;
inline constexpr uint32_t mouse_button_7 = ::atlas::event::Mouse::Button7;
inline constexpr uint32_t mouse_button_last = ::atlas::event::Mouse::ButtonLast;
inline constexpr uint32_t mouse_button_left = ::atlas::event::Mouse::ButtonLeft;
inline constexpr uint32_t mouse_button_right =
  ::atlas::event::Mouse::ButtonRight;
inline constexpr uint32_t mouse_button_middle =
  ::atlas::event::Mouse::ButtonMiddle;