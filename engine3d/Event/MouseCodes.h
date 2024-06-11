#pragma once
#include <cstdint>

namespace engine3d{
	typedef enum class MouseCode : uint16_t{
		// From glfw3.h
		Button0                = 0,
		Button1                = 1,
		Button2                = 2,
		Button3                = 3,
		Button4                = 4,
		Button5                = 5,
		Button6                = 6,
		Button7                = 7,

		ButtonLast             = Button7,
		ButtonLeft             = Button0,
		ButtonRight            = Button1,
		ButtonMiddle           = Button2
	} Mouse;
	
	// inline std::ostream& operator<<(std::ostream& os, MouseCode mouseCode)
	// {
	// 	os << static_cast<int32_t>(mouseCode);
	// 	return os;
	// }
}

#define ENGINE_MOUSE_BUTTON_0      ::engine3d::Mouse::Button0
#define ENGINE_MOUSE_BUTTON_1      ::engine3d::Mouse::Button1
#define ENGINE_MOUSE_BUTTON_2      ::engine3d::Mouse::Button2
#define ENGINE_MOUSE_BUTTON_3      ::engine3d::Mouse::Button3
#define ENGINE_MOUSE_BUTTON_4      ::engine3d::Mouse::Button4
#define ENGINE_MOUSE_BUTTON_5      ::engine3d::Mouse::Button5
#define ENGINE_MOUSE_BUTTON_6      ::engine3d::Mouse::Button6
#define ENGINE_MOUSE_BUTTON_7      ::engine3d::Mouse::Button7
#define ENGINE_MOUSE_BUTTON_LAST   ::engine3d::Mouse::ButtonLast
#define ENGINE_MOUSE_BUTTON_LEFT   ::engine3d::Mouse::ButtonLeft
#define ENGINE_MOUSE_BUTTON_RIGHT  ::engine3d::Mouse::ButtonRight
#define ENGINE_MOUSE_BUTTON_MIDDLE ::engine3d::Mouse::ButtonMiddle
