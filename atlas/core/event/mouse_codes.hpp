#pragma once
#include <cstdint>

namespace atlas{
	enum class MouseCode : uint16_t{
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
	};
	
	// inline std::ostream& operator<<(std::ostream& os, MouseCode mouseCode)
	// {
	// 	os << static_cast<int32_t>(mouseCode);
	// 	return os;
	// }
}

#define MOUSE_BUTTON_0      ::atlas::MouseCode::Button0
#define MOUSE_BUTTON_1      ::atlas::MouseCode::Button1
#define MOUSE_BUTTON_2      ::atlas::MouseCode::Button2
#define MOUSE_BUTTON_3      ::atlas::MouseCode::Button3
#define MOUSE_BUTTON_4      ::atlas::MouseCode::Button4
#define MOUSE_BUTTON_5      ::atlas::MouseCode::Button5
#define MOUSE_BUTTON_6      ::atlas::MouseCode::Button6
#define MOUSE_BUTTON_7      ::atlas::MouseCode::Button7
#define MOUSE_BUTTON_LAST   ::atlas::MouseCode::ButtonLast
#define MOUSE_BUTTON_LEFT   ::atlas::MouseCode::ButtonLeft
#define MOUSE_BUTTON_RIGHT  ::atlas::MouseCode::ButtonRight
#define MOUSE_BUTTON_MIDDLE ::atlas::MouseCode::ButtonMiddle