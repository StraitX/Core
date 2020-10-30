#ifndef STRAITX_LINUX_KEYS_HPP
#define STRATIX_LINUX_KEYS_HPP

#include "platform/keyboard.hpp"
#include <X11/Xlib.h>


namespace StraitX{
namespace Linux{

KeySym StraitXKeyToXKeySym(Keyboard::KeyCode code);

};// namespace Linux::
};// namespace StraitX::

#endif