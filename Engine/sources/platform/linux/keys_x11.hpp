#ifndef STRAITX_LINUX_KEYS_HPP
#define STRATIX_LINUX_KEYS_HPP

#include "platform/keyboard.hpp"
#include "platform/mouse.hpp"
#include <X11/Xlib.h>


namespace StraitX{
namespace Linux{

KeySym StraitXKeyToXKeySym(Keyboard::KeyCode code);

Keyboard::KeyCode XKeyCodeToStraitX(unsigned int key);

Mouse::Button XButtonToStraitX(unsigned int button);

};// namespace Linux::
};// namespace StraitX::

#endif