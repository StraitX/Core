#ifndef STRAITX_WINDOWS_VIRTUAL_KEYS_HPP
#define STRAITX_WINDOWS_VIRTUAL_KEYS_HPP

#include "platform/mouse.hpp"
#include "platform/keyboard.hpp"

namespace StraitX {
namespace Windows {

short MouseButtonToVirtualKey(Mouse::Button button);
short KeyCodeToVirtualKey(Keyboard::KeyCode code);

}; // namespace Windows::
}; // namespace StraitX::

#endif // STRAITX_WINDOWS_VIRTUAL_KEYS_HPP