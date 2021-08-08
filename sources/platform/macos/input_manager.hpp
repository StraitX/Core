#ifndef STRAITX_MACOS_INPUT_MANAGER_HPP
#define STRAITX_MACOS_INPUT_MANAGER_HPP

#include "core/os/keyboard.hpp"
#include "core/os/mouse.hpp"

namespace MacOS{

struct InputManager{
    static bool s_KeyboardState[(size_t)Key::KeysCount];
    static bool s_MouseState[(size_t)Mouse::Button::ButtonCount];

    static void ResetKeyboardState();

    static void ResetMouseState();
};

}//namespace MacOS::

#endif//STRAITX_MACOS_INPUT_MANAGER_HPP