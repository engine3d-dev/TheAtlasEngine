#pragma once
#include <cstdint>

namespace atlas {
    enum KeyCode : int {
        // From glfw3.h
        Space = 32,
        Apostrophe = 39, /* ' */
        Comma = 44,      /* , */
        Minus = 45,      /* - */
        Period = 46,     /* . */
        Slash = 47,      /* / */

        D0 = 48, /* 0 */
        D1 = 49, /* 1 */
        D2 = 50, /* 2 */
        D3 = 51, /* 3 */
        D4 = 52, /* 4 */
        D5 = 53, /* 5 */
        D6 = 54, /* 6 */
        D7 = 55, /* 7 */
        D8 = 56, /* 8 */
        D9 = 57, /* 9 */

        Semicolon = 59, /* ; */
        Equal = 61,     /* = */

        A = 65,
        B = 66,
        C = 67,
        D = 68,
        E = 69,
        F = 70,
        G = 71,
        H = 72,
        I = 73,
        J = 74,
        K = 75,
        L = 76,
        M = 77,
        N = 78,
        O = 79,
        P = 80,
        Q = 81,
        R = 82,
        S = 83,
        T = 84,
        U = 85,
        V = 86,
        W = 87,
        X = 88,
        Y = 89,
        Z = 90,

        LeftBracket = 91,  /* [ */
        Backslash = 92,    /* \ */
        RightBracket = 93, /* ] */
        GraveAccent = 96,  /* ` */

        World1 = 161, /* non-US #1 */
        World2 = 162, /* non-US #2 */

        /* Function keys */
        Escape = 256,
        Enter = 257,
        Tab = 258,
        Backspace = 259,
        Insert = 260,
        Delete = 261,
        Right = 262,
        Left = 263,
        Down = 264,
        Up = 265,
        PageUp = 266,
        PageDown = 267,
        Home = 268,
        End = 269,
        CapsLock = 280,
        ScrollLock = 281,
        NumLock = 282,
        PrintScreen = 283,
        Pause = 284,
        F1 = 290,
        F2 = 291,
        F3 = 292,
        F4 = 293,
        F5 = 294,
        F6 = 295,
        F7 = 296,
        F8 = 297,
        F9 = 298,
        F10 = 299,
        F11 = 300,
        F12 = 301,
        F13 = 302,
        F14 = 303,
        F15 = 304,
        F16 = 305,
        F17 = 306,
        F18 = 307,
        F19 = 308,
        F20 = 309,
        F21 = 310,
        F22 = 311,
        F23 = 312,
        F24 = 313,
        F25 = 314,

        /* Keypad */
        KP0 = 320,
        KP1 = 321,
        KP2 = 322,
        KP3 = 323,
        KP4 = 324,
        KP5 = 325,
        KP6 = 326,
        KP7 = 327,
        KP8 = 328,
        KP9 = 329,
        KPDecimal = 330,
        KPDivide = 331,
        KPMultiply = 332,
        KPSubtract = 333,
        KPAdd = 334,
        KPEnter = 335,
        KPEqual = 336,

        LeftShift = 340,
        LeftControl = 341,
        LeftAlt = 342,
        Leftsuper = 343,
        RightShift = 344,
        RightControl = 345,
        RightAlt = 346,
        Rightsuper = 347,
        Menu = 348
    };
}

// From glfw3.h

inline constexpr int KEY_SPACE = ::atlas::KeyCode::Space;
inline constexpr int KEY_APOSTROPHE = ::atlas::KeyCode::Apostrophe; /* ' */
inline constexpr int KEY_COMMA = ::atlas::KeyCode::Comma;           /* , */
inline constexpr int KEY_MINUS = ::atlas::KeyCode::Minus;           /* - */
inline constexpr int KEY_PERIOD = ::atlas::KeyCode::Period;         /* . */
inline constexpr int KEY_SLASH = ::atlas::KeyCode::Slash;           /* / */
inline constexpr int KEY_0 = ::atlas::KeyCode::D0;
inline constexpr int KEY_1 = ::atlas::KeyCode::D1;
inline constexpr int KEY_2 = ::atlas::KeyCode::D2;
inline constexpr int KEY_3 = ::atlas::KeyCode::D3;
inline constexpr int KEY_4 = ::atlas::KeyCode::D4;
inline constexpr int KEY_5 = ::atlas::KeyCode::D5;
inline constexpr int KEY_6 = ::atlas::KeyCode::D6;
inline constexpr int KEY_7 = ::atlas::KeyCode::D7;
inline constexpr int KEY_8 = ::atlas::KeyCode::D8;
inline constexpr int KEY_9 = ::atlas::KeyCode::D9;
inline constexpr int KEY_SEMICOLON = ::atlas::KeyCode::Semicolon; /* ; */
inline constexpr int KEY_EQUAL = ::atlas::KeyCode::Equal;         /* = */
inline constexpr int KEY_A = ::atlas::KeyCode::A;
inline constexpr int KEY_B = ::atlas::KeyCode::B;
inline constexpr int KEY_C = ::atlas::KeyCode::C;
inline constexpr int KEY_D = ::atlas::KeyCode::D;
inline constexpr int KEY_E = ::atlas::KeyCode::E;
inline constexpr int KEY_F = ::atlas::KeyCode::F;
inline constexpr int KEY_G = ::atlas::KeyCode::G;
inline constexpr int KEY_H = ::atlas::KeyCode::H;
inline constexpr int KEY_I = ::atlas::KeyCode::I;
inline constexpr int KEY_J = ::atlas::KeyCode::J;
inline constexpr int KEY_K = ::atlas::KeyCode::K;
inline constexpr int KEY_L = ::atlas::KeyCode::L;
inline constexpr int KEY_M = ::atlas::KeyCode::M;
inline constexpr int KEY_N = ::atlas::KeyCode::N;
inline constexpr int KEY_O = ::atlas::KeyCode::O;
inline constexpr int KEY_P = ::atlas::KeyCode::P;
inline constexpr int KEY_Q = ::atlas::KeyCode::Q;
inline constexpr int KEY_R = ::atlas::KeyCode::R;
inline constexpr int KEY_S = ::atlas::KeyCode::S;
inline constexpr int KEY_T = ::atlas::KeyCode::T;
inline constexpr int KEY_U = ::atlas::KeyCode::U;
inline constexpr int KEY_V = ::atlas::KeyCode::V;
inline constexpr int KEY_W = ::atlas::KeyCode::W;
inline constexpr int KEY_X = ::atlas::KeyCode::X;
inline constexpr int KEY_Y = ::atlas::KeyCode::Y;
inline constexpr int KEY_Z = ::atlas::KeyCode::Z;
inline constexpr int KEY_LEFT_BRACKET = ::atlas::KeyCode::LeftBracket;   /* [ */
inline constexpr int KEY_BACKSLASH = ::atlas::KeyCode::Backslash;        /* \ */
inline constexpr int KEY_RIGHT_BRACKET = ::atlas::KeyCode::RightBracket; /* ] */
inline constexpr int KEY_GRAVE_ACCENT = ::atlas::KeyCode::GraveAccent;   /* ` */
inline constexpr int KEY_WORLD_1 = ::atlas::KeyCode::World1;             /* non-US #1 */
inline constexpr int KEY_WORLD_2 = ::atlas::KeyCode::World2;             /* non-US #2 */

/* Function keys */
inline constexpr int KEY_ESCAPE = ::atlas::KeyCode::Escape;
inline constexpr int KEY_ENTER = ::atlas::KeyCode::Enter;
inline constexpr int KEY_TAB = ::atlas::KeyCode::Tab;
inline constexpr int KEY_BACKSPACE = ::atlas::KeyCode::Backspace;
inline constexpr int KEY_INSERT = ::atlas::KeyCode::Insert;
inline constexpr int KEY_DELETE = ::atlas::KeyCode::Delete;
inline constexpr int KEY_RIGHT = ::atlas::KeyCode::Right;
inline constexpr int KEY_LEFT = ::atlas::KeyCode::Left;
inline constexpr int KEY_DOWN = ::atlas::KeyCode::Down;
inline constexpr int KEY_UP = ::atlas::KeyCode::Up;
inline constexpr int KEY_PAGE_UP = ::atlas::KeyCode::PageUp;
inline constexpr int KEY_PAGE_DOWN = ::atlas::KeyCode::PageDown;
inline constexpr int KEY_HOME = ::atlas::KeyCode::Home;
inline constexpr int KEY_END = ::atlas::KeyCode::End;
inline constexpr int KEY_CAPS_LOCK = ::atlas::KeyCode::CapsLock;
inline constexpr int KEY_SCROLL_LOCK = ::atlas::KeyCode::ScrollLock;
inline constexpr int KEY_NUM_LOCK = ::atlas::KeyCode::NumLock;
inline constexpr int KEY_PRINT_SCREEN = ::atlas::KeyCode::PrintScreen;
inline constexpr int KEY_PAUSE = ::atlas::KeyCode::Pause;
inline constexpr int KEY_F1 = ::atlas::KeyCode::F1;
inline constexpr int KEY_F2 = ::atlas::KeyCode::F2;
inline constexpr int KEY_F3 = ::atlas::KeyCode::F3;
inline constexpr int KEY_F4 = ::atlas::KeyCode::F4;
inline constexpr int KEY_F5 = ::atlas::KeyCode::F5;
inline constexpr int KEY_F6 = ::atlas::KeyCode::F6;
inline constexpr int KEY_F7 = ::atlas::KeyCode::F7;
inline constexpr int KEY_F8 = ::atlas::KeyCode::F8;
inline constexpr int KEY_F9 = ::atlas::KeyCode::F9;
inline constexpr int KEY_F10 = ::atlas::KeyCode::F10;
inline constexpr int KEY_F11 = ::atlas::KeyCode::F11;
inline constexpr int KEY_F12 = ::atlas::KeyCode::F12;
inline constexpr int KEY_F13 = ::atlas::KeyCode::F13;
inline constexpr int KEY_F14 = ::atlas::KeyCode::F14;
inline constexpr int KEY_F15 = ::atlas::KeyCode::F15;
inline constexpr int KEY_F16 = ::atlas::KeyCode::F16;
inline constexpr int KEY_F17 = ::atlas::KeyCode::F17;
inline constexpr int KEY_F18 = ::atlas::KeyCode::F18;
inline constexpr int KEY_F19 = ::atlas::KeyCode::F19;
inline constexpr int KEY_F20 = ::atlas::KeyCode::F20;
inline constexpr int KEY_F21 = ::atlas::KeyCode::F21;
inline constexpr int KEY_F22 = ::atlas::KeyCode::F22;
inline constexpr int KEY_F23 = ::atlas::KeyCode::F23;
inline constexpr int KEY_F24 = ::atlas::KeyCode::F24;
inline constexpr int KEY_F25 = ::atlas::KeyCode::F25;

/* Keypad */
inline constexpr int KEY_KP_0 = ::atlas::KeyCode::KP0;
inline constexpr int KEY_KP_1 = ::atlas::KeyCode::KP1;
inline constexpr int KEY_KP_2 = ::atlas::KeyCode::KP2;
inline constexpr int KEY_KP_3 = ::atlas::KeyCode::KP3;
inline constexpr int KEY_KP_4 = ::atlas::KeyCode::KP4;
inline constexpr int KEY_KP_5 = ::atlas::KeyCode::KP5;
inline constexpr int KEY_KP_6 = ::atlas::KeyCode::KP6;
inline constexpr int KEY_KP_7 = ::atlas::KeyCode::KP7;
inline constexpr int KEY_KP_8 = ::atlas::KeyCode::KP8;
inline constexpr int KEY_KP_9 = ::atlas::KeyCode::KP9;
inline constexpr int KEY_KP_DECIMAL = ::atlas::KeyCode::KPDecimal;
inline constexpr int KEY_KP_DIVIDE = ::atlas::KeyCode::KPDivide;
inline constexpr int KEY_KP_MULTIPLY = ::atlas::KeyCode::KPMultiply;
inline constexpr int KEY_KP_SUBTRACT = ::atlas::KeyCode::KPSubtract;
inline constexpr int KEY_KP_ADD = ::atlas::KeyCode::KPAdd;
inline constexpr int KEY_KP_ENTER = ::atlas::KeyCode::KPEnter;
inline constexpr int KEY_KP_EQUAL = ::atlas::KeyCode::KPEqual;

inline constexpr int KEY_LEFT_SHIFT = ::atlas::KeyCode::LeftShift;
inline constexpr int KEY_LEFT_CONTROL = ::atlas::KeyCode::LeftControl;
inline constexpr int KEY_LEFT_ALT = ::atlas::KeyCode::LeftAlt;
inline constexpr int KEY_LEFT_SUPER = ::atlas::KeyCode::Leftsuper;
inline constexpr int KEY_RIGHT_SHIFT = ::atlas::KeyCode::RightShift;
inline constexpr int KEY_RIGHT_CONTROL = ::atlas::KeyCode::RightControl;
inline constexpr int KEY_RIGHT_ALT = ::atlas::KeyCode::RightAlt;
inline constexpr int KEY_RIGHT_SUPER = ::atlas::KeyCode::Rightsuper;
inline constexpr int KEY_MENU = ::atlas::KeyCode::Menu;