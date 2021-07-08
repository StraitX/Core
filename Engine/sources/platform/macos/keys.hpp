#ifndef STRAITX_MACOS_KEYS_HPP
#define STRAITX_MACOS_KEYS_HPP

#include "platform/keyboard.hpp"

namespace MacOS{

enum MacOSKeyCode{
    KEY_A = 0,
    KEY_S = 1,
    KEY_D = 2,
    KEY_F = 3,
    KEY_H = 4,
    KEY_G = 5,
    KEY_Z = 6,
    KEY_X = 7,
    KEY_C = 8,
    KEY_V = 9,
    KEY_B = 11,
    KEY_Q = 12,
    KEY_W = 13,
    KEY_E = 14,
    KEY_R = 15,
    KEY_Y = 16,
    KEY_T = 17,
    KEY_1 = 18,
    KEY_2 = 19,
    KEY_3 = 20,
    KEY_4 = 21,
    KEY_6 = 22,
    KEY_5 = 23,
    KEY_EQUALS = 24,
    KEY_9 = 25,
    KEY_7 = 26,
    KEY_MINUS = 27,
    KEY_8 = 28,
    KEY_0 = 29,
    KEY_RIGHTBRACKET = 30,
    KEY_O = 31,
    KEY_U = 32,
    KEY_LEFTBRACKET = 33,
    KEY_I = 34,
    KEY_P = 35,
    KEY_RETURN = 36,
    KEY_L = 37,
    KEY_J = 38,
    KEY_APOSTROPHE = 39,
    KEY_K = 40,
    KEY_SEMICOLON = 41,
    KEY_FRONTSLASH = 42,
    KEY_COMMA = 43,
    KEY_BACKSLASH = 44,
    KEY_N = 45,
    KEY_M = 46,
    KEY_PERIOD = 47,
    KEY_TAB = 48,
    KEY_SPACE = 49,
    KEY_BACKAPOSTROPHE = 50,
    KEY_DELETE_BACKSPACE = 51,
    KEY_ESCAPE = 53,
    KEY_RIGHTCOMMAND = 54,
    KEY_LEFTCOMMAND = 55,
    KEY_LEFTSHIFT = 56,
    KEY_CAPSLOCK = 57,
    KEY_LEFTALT = 58,
    KEY_LEFTCONTROL = 59,
    KEY_RIGHTSHIFHT = 60,
    KEY_RIGHTALT = 61,
    KEY_RIGHTCTRL = 62,
    KEY_NUMPAD_DEL = 65,
    KEY_NUMPAD_MULT = 67,
    KEY_NUMPAD_PLUS = 69,
    KEY_NUMLOCK = 71,
    KEY_NUMPAD_DIV = 75,
    KEY_NUMPAD_ENTER = 76,
    KEY_NUMPAD_MINUS = 78,
    KEY_NUMPAD_0 = 82,
    KEY_NUMPAD_1 = 83,
    KEY_NUMPAD_2 = 84,
    KEY_NUMPAD_3 = 85,
    KEY_NUMPAD_4 = 86,
    KEY_NUMPAD_5 = 87,
    KEY_NUMPAD_6 = 88,
    KEY_NUMPAD_7 = 89,
    KEY_NUMPAD_8 = 91,
    KEY_NUMPAD_9 = 92,
    KEY_MENU = 110,
    KEY_DELETE = 117,
    KEY_HOME = 115,
    KEY_END = 119,
    KEY_PAGEUP = 116,
    KEY_PAGEDOWN = 121,
    KEY_UP = 126,
    KEY_DOWN = 125,
    KEY_LEFT = 123,
    KEY_RIGHT = 124,
    KEY_F1 = 122,
    KEY_F2 = 120,
    KEY_F3 = 99,
    KEY_F4 = 118,
    KEY_F5 = 96,
    KEY_F6 = 97,
    KEY_F7 = 98,
    KEY_F8 = 100,
    KEY_F9 = 101,
    KEY_F10 = 109,
    KEY_F11 = 103,
    KEY_F12 = 111,
    KEY_F13 = 105,
    KEY_F14 = 107,
    KEY_F15 = 113
};

Key ToStraitXKeyCode(int key_code);

}//namespace MacOS::

#endif//STRAITX_MACOS_KEYS_HPP