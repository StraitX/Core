#ifndef STRAITX_MACOS_SX_APPLICATION_HPP
#define STRAITX_MACOS_SX_APPLICATION_HPP

#include "platform/keyboard.hpp"
#include "platform/mouse.hpp"
#include "platform/compiler.hpp"

namespace MacOS{

struct SXApplication{
    static bool s_KeyboardState[(size_t)Key::KeysCount];
    static bool s_MouseState[Mouse::ButtonCount];

    static void ProcessEvents();
};

}//namespace MacOS::

#endif//STRAITX_MACOS_SX_APPLICATION_HPP