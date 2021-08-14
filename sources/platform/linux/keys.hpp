#ifndef STRAITX_LINUX_KEYS_HPP
#define STRAITX_LINUX_KEYS_HPP

#include "core/os/keyboard.hpp"
#include "core/os/mouse.hpp"
#include "platform/linux/x11.hpp"

namespace Linux{

X11::KeySym KeyCodeToXKeySym(Key code);

Key XKeyCodeToKeyCode(unsigned int key);

Mouse::Button XButtonToMouseButton(unsigned int button);

}//namespace Linux::

#endif