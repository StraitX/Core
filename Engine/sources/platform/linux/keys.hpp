#ifndef STRAITX_LINUX_KEYS_HPP
#define STRAITX_LINUX_KEYS_HPP

#include "platform/keyboard.hpp"
#include "platform/mouse.hpp"
#include <X11/Xlib.h>


namespace StraitX{
namespace Linux{

KeySym KeyCodeToXKeySym(Keyboard::KeyCode code);

Keyboard::KeyCode XKeyCodeToKeyCode(unsigned int key);

Mouse::Button XButtonToMouseButton(unsigned int button);

}// namespace Linux::
}// namespace StraitX::

#endif