#pragma once
#include <cstdint>

namespace atlas {
    enum class KeyCode : uint16_t {
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
#define KEY_SPACE ::atlas::KeyCode::Space
#define KEY_APOSTROPHE ::atlas::KeyCode::Apostrophe /* ' */
#define KEY_COMMA ::atlas::KeyCode::Comma           /* , */
#define KEY_MINUS ::atlas::KeyCode::Minus           /* - */
#define KEY_PERIOD ::atlas::KeyCode::Period         /* . */
#define KEY_SLASH ::atlas::KeyCode::Slash           /* / */
#define KEY_0 ::atlas::KeyCode::D0
#define KEY_1 ::atlas::KeyCode::D1
#define KEY_2 ::atlas::KeyCode::D2
#define KEY_3 ::atlas::KeyCode::D3
#define KEY_4 ::atlas::KeyCode::D4
#define KEY_5 ::atlas::KeyCode::D5
#define KEY_6 ::atlas::KeyCode::D6
#define KEY_7 ::atlas::KeyCode::D7
#define KEY_8 ::atlas::KeyCode::D8
#define KEY_9 ::atlas::KeyCode::D9
#define KEY_SEMICOLON ::atlas::KeyCode::Semicolon /* ; */
#define KEY_EQUAL ::atlas::KeyCode::Equal         /* = */
#define KEY_A ::atlas::KeyCode::A
#define KEY_B ::atlas::KeyCode::B
#define KEY_C ::atlas::KeyCode::C
#define KEY_D ::atlas::KeyCode::D
#define KEY_E ::atlas::KeyCode::E
#define KEY_F ::atlas::KeyCode::F
#define KEY_G ::atlas::KeyCode::G
#define KEY_H ::atlas::KeyCode::H
#define KEY_I ::atlas::KeyCode::I
#define KEY_J ::atlas::KeyCode::J
#define KEY_K ::atlas::KeyCode::K
#define KEY_L ::atlas::KeyCode::L
#define KEY_M ::atlas::KeyCode::M
#define KEY_N ::atlas::KeyCode::N
#define KEY_O ::atlas::KeyCode::O
#define KEY_P ::atlas::KeyCode::P
#define KEY_Q ::atlas::KeyCode::Q
#define KEY_R ::atlas::KeyCode::R
#define KEY_S ::atlas::KeyCode::S
#define KEY_T ::atlas::KeyCode::T
#define KEY_U ::atlas::KeyCode::U
#define KEY_V ::atlas::KeyCode::V
#define KEY_W ::atlas::KeyCode::W
#define KEY_X ::atlas::KeyCode::X
#define KEY_Y ::atlas::KeyCode::Y
#define KEY_Z ::atlas::KeyCode::Z
#define KEY_LEFT_BRACKET ::atlas::KeyCode::LeftBracket   /* [ */
#define KEY_BACKSLASH ::atlas::KeyCode::Backslash        /* \ */
#define KEY_RIGHT_BRACKET ::atlas::KeyCode::RightBracket /* ] */
#define KEY_GRAVE_ACCENT ::atlas::KeyCode::GraveAccent   /* ` */
#define KEY_WORLD_1 ::atlas::KeyCode::World1             /* non-US #1 */
#define KEY_WORLD_2 ::atlas::KeyCode::World2             /* non-US #2 */

/* Function keys */
#define KEY_ESCAPE ::atlas::KeyCode::Escape
#define KEY_ENTER ::atlas::KeyCode::Enter
#define KEY_TAB ::atlas::KeyCode::Tab
#define KEY_BACKSPACE ::atlas::KeyCode::Backspace
#define KEY_INSERT ::atlas::KeyCode::Insert
#define KEY_DELETE ::atlas::KeyCode::Delete
#define KEY_RIGHT ::atlas::KeyCode::Right
#define KEY_LEFT ::atlas::KeyCode::Left
#define KEY_DOWN ::atlas::KeyCode::Down
#define KEY_UP ::atlas::KeyCode::Up
#define KEY_PAGE_UP ::atlas::KeyCode::PageUp
#define KEY_PAGE_DOWN ::atlas::KeyCode::PageDown
#define KEY_HOME ::atlas::KeyCode::Home
#define KEY_END ::atlas::KeyCode::End
#define KEY_CAPS_LOCK ::atlas::KeyCode::CapsLock
#define KEY_SCROLL_LOCK ::atlas::KeyCode::ScrollLock
#define KEY_NUM_LOCK ::atlas::KeyCode::NumLock
#define KEY_PRINT_SCREEN ::atlas::KeyCode::PrintScreen
#define KEY_PAUSE ::atlas::KeyCode::Pause
#define KEY_F1 ::atlas::KeyCode::F1
#define KEY_F2 ::atlas::KeyCode::F2
#define KEY_F3 ::atlas::KeyCode::F3
#define KEY_F4 ::atlas::KeyCode::F4
#define KEY_F5 ::atlas::KeyCode::F5
#define KEY_F6 ::atlas::KeyCode::F6
#define KEY_F7 ::atlas::KeyCode::F7
#define KEY_F8 ::atlas::KeyCode::F8
#define KEY_F9 ::atlas::KeyCode::F9
#define KEY_F10 ::atlas::KeyCode::F10
#define KEY_F11 ::atlas::KeyCode::F11
#define KEY_F12 ::atlas::KeyCode::F12
#define KEY_F13 ::atlas::KeyCode::F13
#define KEY_F14 ::atlas::KeyCode::F14
#define KEY_F15 ::atlas::KeyCode::F15
#define KEY_F16 ::atlas::KeyCode::F16
#define KEY_F17 ::atlas::KeyCode::F17
#define KEY_F18 ::atlas::KeyCode::F18
#define KEY_F19 ::atlas::KeyCode::F19
#define KEY_F20 ::atlas::KeyCode::F20
#define KEY_F21 ::atlas::KeyCode::F21
#define KEY_F22 ::atlas::KeyCode::F22
#define KEY_F23 ::atlas::KeyCode::F23
#define KEY_F24 ::atlas::KeyCode::F24
#define KEY_F25 ::atlas::KeyCode::F25

/* Keypad */
#define KEY_KP_0 ::atlas::KeyCode::KP0
#define KEY_KP_1 ::atlas::KeyCode::KP1
#define KEY_KP_2 ::atlas::KeyCode::KP2
#define KEY_KP_3 ::atlas::KeyCode::KP3
#define KEY_KP_4 ::atlas::KeyCode::KP4
#define KEY_KP_5 ::atlas::KeyCode::KP5
#define KEY_KP_6 ::atlas::KeyCode::KP6
#define KEY_KP_7 ::atlas::KeyCode::KP7
#define KEY_KP_8 ::atlas::KeyCode::KP8
#define KEY_KP_9 ::atlas::KeyCode::KP9
#define KEY_KP_DECIMAL ::atlas::KeyCode::KPDecimal
#define KEY_KP_DIVIDE ::atlas::KeyCode::KPDivide
#define KEY_KP_MULTIPLY ::atlas::KeyCode::KPMultiply
#define KEY_KP_SUBTRACT ::atlas::KeyCode::KPSubtract
#define KEY_KP_ADD ::atlas::KeyCode::KPAdd
#define KEY_KP_ENTER ::atlas::KeyCode::KPEnter
#define KEY_KP_EQUAL ::atlas::KeyCode::KPEqual

#define KEY_LEFT_SHIFT ::atlas::KeyCode::LeftShift
#define KEY_LEFT_CONTROL ::atlas::KeyCode::LeftControl
#define KEY_LEFT_ALT ::atlas::KeyCode::LeftAlt
#define KEY_LEFT_SUPER ::atlas::KeyCode::Leftsuper
#define KEY_RIGHT_SHIFT ::atlas::KeyCode::RightShift
#define KEY_RIGHT_CONTROL ::atlas::KeyCode::RightControl
#define KEY_RIGHT_ALT ::atlas::KeyCode::RightAlt
#define KEY_RIGHT_SUPER ::atlas::KeyCode::Rightsuper
#define KEY_MENU ::atlas::KeyCode::Menu