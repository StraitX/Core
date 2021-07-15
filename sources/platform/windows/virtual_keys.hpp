#ifndef STRAITX_WINDOWS_VIRTUAL_KEYS_HPP
#define STRAITX_WINDOWS_VIRTUAL_KEYS_HPP

#include "platform/mouse.hpp"
#include "platform/keyboard.hpp"
#include <windows.h>

namespace Windows {

short MouseButtonToVirtualKey(Mouse::Button button);

short KeyCodeToVirtualKey(Key code);

short MouseButtonToVirtualKey(Mouse::Button button);

Key VirtualKeyToKeyCode(short code);

Key VirtualKeyExtendedToKeyCode(WPARAM key, LPARAM flags);

}//namespace Windows::

#endif // STRAITX_WINDOWS_VIRTUAL_KEYS_HPP