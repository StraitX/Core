#ifndef STRAITX_WINDOWS_VIRTUAL_KEYS_HPP
#define STRAITX_WINDOWS_VIRTUAL_KEYS_HPP

#include "platform/mouse.hpp"
#include "platform/keyboard.hpp"
#include <windows.h>

namespace StraitX {
namespace Windows {

short MouseButtonToVirtualKey(Mouse::Button button);
short KeyCodeToVirtualKey(Keyboard::KeyCode code);

short MouseButtonToVirtualKey(Mouse::Button button);
Keyboard::KeyCode VirtualKeyToKeyCode(short code);
Keyboard::KeyCode VirtualKeyExtendedToKeyCode(WPARAM key, LPARAM flags);

}; // namespace Windows::
}; // namespace StraitX::

#endif // STRAITX_WINDOWS_VIRTUAL_KEYS_HPP