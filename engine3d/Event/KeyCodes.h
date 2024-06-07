#pragma once
#include <cstdint>

namespace Engine3D{
	typedef enum class KeyCode : uint16_t {
		// From glfw3.h
		Space               = 32,
		Apostrophe          = 39, /* ' */
		Comma               = 44, /* , */
		Minus               = 45, /* - */
		Period              = 46, /* . */
		Slash               = 47, /* / */

		D0                  = 48, /* 0 */
		D1                  = 49, /* 1 */
		D2                  = 50, /* 2 */
		D3                  = 51, /* 3 */
		D4                  = 52, /* 4 */
		D5                  = 53, /* 5 */
		D6                  = 54, /* 6 */
		D7                  = 55, /* 7 */
		D8                  = 56, /* 8 */
		D9                  = 57, /* 9 */

		Semicolon           = 59, /* ; */
		Equal               = 61, /* = */

		A                   = 65,
		B                   = 66,
		C                   = 67,
		D                   = 68,
		E                   = 69,
		F                   = 70,
		G                   = 71,
		H                   = 72,
		I                   = 73,
		J                   = 74,
		K                   = 75,
		L                   = 76,
		M                   = 77,
		N                   = 78,
		O                   = 79,
		P                   = 80,
		Q                   = 81,
		R                   = 82,
		S                   = 83,
		T                   = 84,
		U                   = 85,
		V                   = 86,
		W                   = 87,
		X                   = 88,
		Y                   = 89,
		Z                   = 90,

		LeftBracket         = 91,  /* [ */
		Backslash           = 92,  /* \ */
		RightBracket        = 93,  /* ] */
		GraveAccent         = 96,  /* ` */

		World1              = 161, /* non-US #1 */
		World2              = 162, /* non-US #2 */

		/* Function keys */
		Escape              = 256,
		Enter               = 257,
		Tab                 = 258,
		Backspace           = 259,
		Insert              = 260,
		Delete              = 261,
		Right               = 262,
		Left                = 263,
		Down                = 264,
		Up                  = 265,
		PageUp              = 266,
		PageDown            = 267,
		Home                = 268,
		End                 = 269,
		CapsLock            = 280,
		ScrollLock          = 281,
		NumLock             = 282,
		PrintScreen         = 283,
		Pause               = 284,
		F1                  = 290,
		F2                  = 291,
		F3                  = 292,
		F4                  = 293,
		F5                  = 294,
		F6                  = 295,
		F7                  = 296,
		F8                  = 297,
		F9                  = 298,
		F10                 = 299,
		F11                 = 300,
		F12                 = 301,
		F13                 = 302,
		F14                 = 303,
		F15                 = 304,
		F16                 = 305,
		F17                 = 306,
		F18                 = 307,
		F19                 = 308,
		F20                 = 309,
		F21                 = 310,
		F22                 = 311,
		F23                 = 312,
		F24                 = 313,
		F25                 = 314,

		/* Keypad */
		KP0                 = 320,
		KP1                 = 321,
		KP2                 = 322,
		KP3                 = 323,
		KP4                 = 324,
		KP5                 = 325,
		KP6                 = 326,
		KP7                 = 327,
		KP8                 = 328,
		KP9                 = 329,
		KPDecimal           = 330,
		KPDivide            = 331,
		KPMultiply          = 332,
		KPSubtract          = 333,
		KPAdd               = 334,
		KPEnter             = 335,
		KPEqual             = 336,

		LeftShift           = 340,
		LeftControl         = 341,
		LeftAlt             = 342,
		LeftSuper           = 343,
		RightShift          = 344,
		RightControl        = 345,
		RightAlt            = 346,
		RightSuper          = 347,
		Menu                = 348
	} Key;
}

// From glfw3.h
#define ENGINE_KEY_SPACE           ::Engine3D::Key::Space
#define ENGINE_KEY_APOSTROPHE      ::Engine3D::Key::Apostrophe    /* ' */
#define ENGINE_KEY_COMMA           ::Engine3D::Key::Comma         /* , */
#define ENGINE_KEY_MINUS           ::Engine3D::Key::Minus         /* - */
#define ENGINE_KEY_PERIOD          ::Engine3D::Key::Period        /* . */
#define ENGINE_KEY_SLASH           ::Engine3D::Key::Slash         /* / */
#define ENGINE_KEY_0               ::Engine3D::Key::D0
#define ENGINE_KEY_1               ::Engine3D::Key::D1
#define ENGINE_KEY_2               ::Engine3D::Key::D2
#define ENGINE_KEY_3               ::Engine3D::Key::D3
#define ENGINE_KEY_4               ::Engine3D::Key::D4
#define ENGINE_KEY_5               ::Engine3D::Key::D5
#define ENGINE_KEY_6               ::Engine3D::Key::D6
#define ENGINE_KEY_7               ::Engine3D::Key::D7
#define ENGINE_KEY_8               ::Engine3D::Key::D8
#define ENGINE_KEY_9               ::Engine3D::Key::D9
#define ENGINE_KEY_SEMICOLON       ::Engine3D::Key::Semicolon     /* ; */
#define ENGINE_KEY_EQUAL           ::Engine3D::Key::Equal         /* = */
#define ENGINE_KEY_A               ::Engine3D::Key::A
#define ENGINE_KEY_B               ::Engine3D::Key::B
#define ENGINE_KEY_C               ::Engine3D::Key::C
#define ENGINE_KEY_D               ::Engine3D::Key::D
#define ENGINE_KEY_E               ::Engine3D::Key::E
#define ENGINE_KEY_F               ::Engine3D::Key::F
#define ENGINE_KEY_G               ::Engine3D::Key::G
#define ENGINE_KEY_H               ::Engine3D::Key::H
#define ENGINE_KEY_I               ::Engine3D::Key::I
#define ENGINE_KEY_J               ::Engine3D::Key::J
#define ENGINE_KEY_K               ::Engine3D::Key::K
#define ENGINE_KEY_L               ::Engine3D::Key::L
#define ENGINE_KEY_M               ::Engine3D::Key::M
#define ENGINE_KEY_N               ::Engine3D::Key::N
#define ENGINE_KEY_O               ::Engine3D::Key::O
#define ENGINE_KEY_P               ::Engine3D::Key::P
#define ENGINE_KEY_Q               ::Engine3D::Key::Q
#define ENGINE_KEY_R               ::Engine3D::Key::R
#define ENGINE_KEY_S               ::Engine3D::Key::S
#define ENGINE_KEY_T               ::Engine3D::Key::T
#define ENGINE_KEY_U               ::Engine3D::Key::U
#define ENGINE_KEY_V               ::Engine3D::Key::V
#define ENGINE_KEY_W               ::Engine3D::Key::W
#define ENGINE_KEY_X               ::Engine3D::Key::X
#define ENGINE_KEY_Y               ::Engine3D::Key::Y
#define ENGINE_KEY_Z               ::Engine3D::Key::Z
#define ENGINE_KEY_LEFT_BRACKET    ::Engine3D::Key::LeftBracket   /* [ */
#define ENGINE_KEY_BACKSLASH       ::Engine3D::Key::Backslash     /* \ */
#define ENGINE_KEY_RIGHT_BRACKET   ::Engine3D::Key::RightBracket  /* ] */
#define ENGINE_KEY_GRAVE_ACCENT    ::Engine3D::Key::GraveAccent   /* ` */
#define ENGINE_KEY_WORLD_1         ::Engine3D::Key::World1        /* non-US #1 */
#define ENGINE_KEY_WORLD_2         ::Engine3D::Key::World2        /* non-US #2 */

/* Function keys */
#define ENGINE_KEY_ESCAPE          ::Engine3D::Key::Escape
#define ENGINE_KEY_ENTER           ::Engine3D::Key::Enter
#define ENGINE_KEY_TAB             ::Engine3D::Key::Tab
#define ENGINE_KEY_BACKSPACE       ::Engine3D::Key::Backspace
#define ENGINE_KEY_INSERT          ::Engine3D::Key::Insert
#define ENGINE_KEY_DELETE          ::Engine3D::Key::Delete
#define ENGINE_KEY_RIGHT           ::Engine3D::Key::Right
#define ENGINE_KEY_LEFT            ::Engine3D::Key::Left
#define ENGINE_KEY_DOWN            ::Engine3D::Key::Down
#define ENGINE_KEY_UP              ::Engine3D::Key::Up
#define ENGINE_KEY_PAGE_UP         ::Engine3D::Key::PageUp
#define ENGINE_KEY_PAGE_DOWN       ::Engine3D::Key::PageDown
#define ENGINE_KEY_HOME            ::Engine3D::Key::Home
#define ENGINE_KEY_END             ::Engine3D::Key::End
#define ENGINE_KEY_CAPS_LOCK       ::Engine3D::Key::CapsLock
#define ENGINE_KEY_SCROLL_LOCK     ::Engine3D::Key::ScrollLock
#define ENGINE_KEY_NUM_LOCK        ::Engine3D::Key::NumLock
#define ENGINE_KEY_PRINT_SCREEN    ::Engine3D::Key::PrintScreen
#define ENGINE_KEY_PAUSE           ::Engine3D::Key::Pause
#define ENGINE_KEY_F1              ::Engine3D::Key::F1
#define ENGINE_KEY_F2              ::Engine3D::Key::F2
#define ENGINE_KEY_F3              ::Engine3D::Key::F3
#define ENGINE_KEY_F4              ::Engine3D::Key::F4
#define ENGINE_KEY_F5              ::Engine3D::Key::F5
#define ENGINE_KEY_F6              ::Engine3D::Key::F6
#define ENGINE_KEY_F7              ::Engine3D::Key::F7
#define ENGINE_KEY_F8              ::Engine3D::Key::F8
#define ENGINE_KEY_F9              ::Engine3D::Key::F9
#define ENGINE_KEY_F10             ::Engine3D::Key::F10
#define ENGINE_KEY_F11             ::Engine3D::Key::F11
#define ENGINE_KEY_F12             ::Engine3D::Key::F12
#define ENGINE_KEY_F13             ::Engine3D::Key::F13
#define ENGINE_KEY_F14             ::Engine3D::Key::F14
#define ENGINE_KEY_F15             ::Engine3D::Key::F15
#define ENGINE_KEY_F16             ::Engine3D::Key::F16
#define ENGINE_KEY_F17             ::Engine3D::Key::F17
#define ENGINE_KEY_F18             ::Engine3D::Key::F18
#define ENGINE_KEY_F19             ::Engine3D::Key::F19
#define ENGINE_KEY_F20             ::Engine3D::Key::F20
#define ENGINE_KEY_F21             ::Engine3D::Key::F21
#define ENGINE_KEY_F22             ::Engine3D::Key::F22
#define ENGINE_KEY_F23             ::Engine3D::Key::F23
#define ENGINE_KEY_F24             ::Engine3D::Key::F24
#define ENGINE_KEY_F25             ::Engine3D::Key::F25

/* Keypad */
#define ENGINE_KEY_KP_0            ::Engine3D::Key::KP0
#define ENGINE_KEY_KP_1            ::Engine3D::Key::KP1
#define ENGINE_KEY_KP_2            ::Engine3D::Key::KP2
#define ENGINE_KEY_KP_3            ::Engine3D::Key::KP3
#define ENGINE_KEY_KP_4            ::Engine3D::Key::KP4
#define ENGINE_KEY_KP_5            ::Engine3D::Key::KP5
#define ENGINE_KEY_KP_6            ::Engine3D::Key::KP6
#define ENGINE_KEY_KP_7            ::Engine3D::Key::KP7
#define ENGINE_KEY_KP_8            ::Engine3D::Key::KP8
#define ENGINE_KEY_KP_9            ::Engine3D::Key::KP9
#define ENGINE_KEY_KP_DECIMAL      ::Engine3D::Key::KPDecimal
#define ENGINE_KEY_KP_DIVIDE       ::Engine3D::Key::KPDivide
#define ENGINE_KEY_KP_MULTIPLY     ::Engine3D::Key::KPMultiply
#define ENGINE_KEY_KP_SUBTRACT     ::Engine3D::Key::KPSubtract
#define ENGINE_KEY_KP_ADD          ::Engine3D::Key::KPAdd
#define ENGINE_KEY_KP_ENTER        ::Engine3D::Key::KPEnter
#define ENGINE_KEY_KP_EQUAL        ::Engine3D::Key::KPEqual

#define ENGINE_KEY_LEFT_SHIFT      ::Engine3D::Key::LeftShift
#define ENGINE_KEY_LEFT_CONTROL    ::Engine3D::Key::LeftControl
#define ENGINE_KEY_LEFT_ALT        ::Engine3D::Key::LeftAlt
#define ENGINE_KEY_LEFT_SUPER      ::Engine3D::Key::LeftSuper
#define ENGINE_KEY_RIGHT_SHIFT     ::Engine3D::Key::RightShift
#define ENGINE_KEY_RIGHT_CONTROL   ::Engine3D::Key::RightControl
#define ENGINE_KEY_RIGHT_ALT       ::Engine3D::Key::RightAlt
#define ENGINE_KEY_RIGHT_SUPER     ::Engine3D::Key::RightSuper
#define ENGINE_KEY_MENU            ::Engine3D::Key::Menu
