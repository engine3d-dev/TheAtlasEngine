#pragma once
#include <cstdint>

namespace atlas::event {
    enum Key : uint32_t {
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

inline constexpr uint32_t key_space = ::atlas::event::Key::Space;
inline constexpr uint32_t key_apostrophe =
  ::atlas::event::Key::Apostrophe;                                  /* ' */
inline constexpr uint32_t key_comma = ::atlas::event::Key::Comma;   /* , */
inline constexpr uint32_t key_minus = ::atlas::event::Key::Minus;   /* - */
inline constexpr uint32_t key_period = ::atlas::event::Key::Period; /* . */
inline constexpr uint32_t key_slash = ::atlas::event::Key::Slash;   /* / */
inline constexpr uint32_t key_0 = ::atlas::event::Key::D0;
inline constexpr uint32_t key_1 = ::atlas::event::Key::D1;
inline constexpr uint32_t key_2 = ::atlas::event::Key::D2;
inline constexpr uint32_t key_3 = ::atlas::event::Key::D3;
inline constexpr uint32_t key_4 = ::atlas::event::Key::D4;
inline constexpr uint32_t key_5 = ::atlas::event::Key::D5;
inline constexpr uint32_t key_6 = ::atlas::event::Key::D6;
inline constexpr uint32_t key_7 = ::atlas::event::Key::D7;
inline constexpr uint32_t key_8 = ::atlas::event::Key::D8;
inline constexpr uint32_t key_9 = ::atlas::event::Key::D9;
inline constexpr uint32_t key_semicolon =
  ::atlas::event::Key::Semicolon;                                 /* ; */
inline constexpr uint32_t key_equal = ::atlas::event::Key::Equal; /* = */
inline constexpr uint32_t key_a = ::atlas::event::Key::A;
inline constexpr uint32_t key_b = ::atlas::event::Key::B;
inline constexpr uint32_t key_c = ::atlas::event::Key::C;
inline constexpr uint32_t key_d = ::atlas::event::Key::D;
inline constexpr uint32_t key_e = ::atlas::event::Key::E;
inline constexpr uint32_t key_f = ::atlas::event::Key::F;
inline constexpr uint32_t key_g = ::atlas::event::Key::G;
inline constexpr uint32_t key_h = ::atlas::event::Key::H;
inline constexpr uint32_t key_i = ::atlas::event::Key::I;
inline constexpr uint32_t key_j = ::atlas::event::Key::J;
inline constexpr uint32_t key_k = ::atlas::event::Key::K;
inline constexpr uint32_t key_l = ::atlas::event::Key::L;
inline constexpr uint32_t key_m = ::atlas::event::Key::M;
inline constexpr uint32_t key_n = ::atlas::event::Key::N;
inline constexpr uint32_t key_o = ::atlas::event::Key::O;
inline constexpr uint32_t key_p = ::atlas::event::Key::P;
inline constexpr uint32_t key_q = ::atlas::event::Key::Q;
inline constexpr uint32_t key_r = ::atlas::event::Key::R;
inline constexpr uint32_t key_s = ::atlas::event::Key::S;
// inline constexpr uint32_t key_t = ::atlas::event::Key::T;
inline constexpr uint32_t key_u = ::atlas::event::Key::U;
inline constexpr uint32_t key_v = ::atlas::event::Key::V;
inline constexpr uint32_t key_w = ::atlas::event::Key::W;
inline constexpr uint32_t key_x = ::atlas::event::Key::X;
inline constexpr uint32_t key_y = ::atlas::event::Key::Y;
inline constexpr uint32_t key_z = ::atlas::event::Key::Z;
inline constexpr uint32_t key_left_bracket =
  ::atlas::event::Key::LeftBracket; /* [ */
inline constexpr uint32_t key_backslash =
  ::atlas::event::Key::Backslash; /* \ */
inline constexpr uint32_t key_right_bracket =
  ::atlas::event::Key::RightBracket; /* ] */
inline constexpr uint32_t key_gravity_accent =
  ::atlas::event::Key::GraveAccent; /* ` */
inline constexpr uint32_t key_world_1 =
  ::atlas::event::Key::World1; /* non-US #1 */
inline constexpr uint32_t key_world_2 =
  ::atlas::event::Key::World2; /* non-US #2 */

/* Function keys */
inline constexpr uint32_t key_escape = ::atlas::event::Key::Escape;
inline constexpr uint32_t key_enter = ::atlas::event::Key::Enter;
inline constexpr uint32_t key_tab = ::atlas::event::Key::Tab;
inline constexpr uint32_t key_backspace = ::atlas::event::Key::Backspace;
inline constexpr uint32_t key_insert = ::atlas::event::Key::Insert;
inline constexpr uint32_t key_delete = ::atlas::event::Key::Delete;
inline constexpr uint32_t key_right = ::atlas::event::Key::Right;
inline constexpr uint32_t key_left = ::atlas::event::Key::Left;
inline constexpr uint32_t key_down = ::atlas::event::Key::Down;
inline constexpr uint32_t key_up = ::atlas::event::Key::Up;
inline constexpr uint32_t key_page_up = ::atlas::event::Key::PageUp;
inline constexpr uint32_t key_page_down = ::atlas::event::Key::PageDown;
inline constexpr uint32_t key_home = ::atlas::event::Key::Home;
inline constexpr uint32_t key_end = ::atlas::event::Key::End;
inline constexpr uint32_t key_caps_lock = ::atlas::event::Key::CapsLock;
inline constexpr uint32_t key_scroll_lock = ::atlas::event::Key::ScrollLock;
inline constexpr uint32_t key_num_lock = ::atlas::event::Key::NumLock;
inline constexpr uint32_t key_print_screen = ::atlas::event::Key::PrintScreen;
inline constexpr uint32_t key_pause = ::atlas::event::Key::Pause;
inline constexpr uint32_t key_f1 = ::atlas::event::Key::F1;
inline constexpr uint32_t key_f2 = ::atlas::event::Key::F2;
inline constexpr uint32_t key_f3 = ::atlas::event::Key::F3;
inline constexpr uint32_t key_f4 = ::atlas::event::Key::F4;
inline constexpr uint32_t key_f5 = ::atlas::event::Key::F5;
inline constexpr uint32_t key_f6 = ::atlas::event::Key::F6;
inline constexpr uint32_t key_f7 = ::atlas::event::Key::F7;
inline constexpr uint32_t key_f8 = ::atlas::event::Key::F8;
inline constexpr uint32_t key_f9 = ::atlas::event::Key::F9;
inline constexpr uint32_t key_f10 = ::atlas::event::Key::F10;
inline constexpr uint32_t key_f11 = ::atlas::event::Key::F11;
inline constexpr uint32_t key_f12 = ::atlas::event::Key::F12;
inline constexpr uint32_t key_f13 = ::atlas::event::Key::F13;
inline constexpr uint32_t key_f14 = ::atlas::event::Key::F14;
inline constexpr uint32_t key_f15 = ::atlas::event::Key::F15;
inline constexpr uint32_t key_f16 = ::atlas::event::Key::F16;
inline constexpr uint32_t key_f17 = ::atlas::event::Key::F17;
inline constexpr uint32_t key_f18 = ::atlas::event::Key::F18;
inline constexpr uint32_t key_f19 = ::atlas::event::Key::F19;
inline constexpr uint32_t key_f20 = ::atlas::event::Key::F20;
inline constexpr uint32_t key_f21 = ::atlas::event::Key::F21;
inline constexpr uint32_t key_f22 = ::atlas::event::Key::F22;
inline constexpr uint32_t key_f23 = ::atlas::event::Key::F23;
inline constexpr uint32_t key_f24 = ::atlas::event::Key::F24;
inline constexpr uint32_t key_f25 = ::atlas::event::Key::F25;

/* Keypad */
inline constexpr uint32_t key_kp_0 = ::atlas::event::Key::KP0;
inline constexpr uint32_t key_kp_1 = ::atlas::event::Key::KP1;
inline constexpr uint32_t key_kp_2 = ::atlas::event::Key::KP2;
inline constexpr uint32_t key_kp_3 = ::atlas::event::Key::KP3;
inline constexpr uint32_t key_kp_4 = ::atlas::event::Key::KP4;
inline constexpr uint32_t key_kp_5 = ::atlas::event::Key::KP5;
inline constexpr uint32_t key_kp_6 = ::atlas::event::Key::KP6;
inline constexpr uint32_t key_kp_7 = ::atlas::event::Key::KP7;
inline constexpr uint32_t key_kp_8 = ::atlas::event::Key::KP8;
inline constexpr uint32_t key_kp_9 = ::atlas::event::Key::KP9;
inline constexpr uint32_t key_kp_decimal = ::atlas::event::Key::KPDecimal;
inline constexpr uint32_t key_kp_divide = ::atlas::event::Key::KPDivide;
inline constexpr uint32_t key_kp_multiply = ::atlas::event::Key::KPMultiply;
inline constexpr uint32_t key_kp_subtract = ::atlas::event::Key::KPSubtract;
inline constexpr uint32_t key_kp_add = ::atlas::event::Key::KPAdd;
inline constexpr uint32_t key_kp_enter = ::atlas::event::Key::KPEnter;
inline constexpr uint32_t key_kp_equal = ::atlas::event::Key::KPEqual;

inline constexpr uint32_t key_left_shift = ::atlas::event::Key::LeftShift;
inline constexpr uint32_t key_left_control = ::atlas::event::Key::LeftControl;
inline constexpr uint32_t key_left_alt = ::atlas::event::Key::LeftAlt;
inline constexpr uint32_t key_left_super = ::atlas::event::Key::Leftsuper;
inline constexpr uint32_t key_right_shift = ::atlas::event::Key::RightShift;
inline constexpr uint32_t key_right_control = ::atlas::event::Key::RightControl;
inline constexpr uint32_t key_right_alt = ::atlas::event::Key::RightAlt;
inline constexpr uint32_t key_right_super = ::atlas::event::Key::Rightsuper;
inline constexpr uint32_t key_menu = ::atlas::event::Key::Menu;