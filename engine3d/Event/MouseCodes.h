#pragma once
#include <cstdint>

namespace Engine3D{
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

#define ENGINE_MOUSE_BUTTON_0      ::Engine3D::Mouse::Button0
#define ENGINE_MOUSE_BUTTON_1      ::Engine3D::Mouse::Button1
#define ENGINE_MOUSE_BUTTON_2      ::Engine3D::Mouse::Button2
#define ENGINE_MOUSE_BUTTON_3      ::Engine3D::Mouse::Button3
#define ENGINE_MOUSE_BUTTON_4      ::Engine3D::Mouse::Button4
#define ENGINE_MOUSE_BUTTON_5      ::Engine3D::Mouse::Button5
#define ENGINE_MOUSE_BUTTON_6      ::Engine3D::Mouse::Button6
#define ENGINE_MOUSE_BUTTON_7      ::Engine3D::Mouse::Button7
#define ENGINE_MOUSE_BUTTON_LAST   ::Engine3D::Mouse::ButtonLast
#define ENGINE_MOUSE_BUTTON_LEFT   ::Engine3D::Mouse::ButtonLeft
#define ENGINE_MOUSE_BUTTON_RIGHT  ::Engine3D::Mouse::ButtonRight
#define ENGINE_MOUSE_BUTTON_MIDDLE ::Engine3D::Mouse::ButtonMiddle
