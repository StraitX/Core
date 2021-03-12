#ifndef STRAITX_MACOS_KEYS_HPP
#define STRAITX_MACOS_KEYS_HPP

#include "platform/keyboard.hpp"

namespace StraitX{
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
    KEY_DELETE = 51,
    KEY_ESCAPE = 53,
    KEY_COMMAND = 55,
    KEY_SHIFT = 56,
    KEY_CAPSLOCK = 57,
    KEY_OPTION = 58,
    KEY_CONTROL = 59,
    KEY_UP = 126,
    KEY_DOWN = 125,
    KEY_LEFT = 123,
    KEY_RIGHT = 124
};

Keyboard::KeyCode ToStraitXKeyCode(int key_code);

}//namespace MacOS
}//namespace StraitX

#endif//STRAITX_MACOS_KEYS_HPP