#ifndef STRAITX_LINUX_EVENTS_HPP
#define STRAITX_LINUX_EVENTS_HPP

#include "platform/events.hpp"
#include <X11/Xlib.h>

namespace StraitX{
namespace Linux{


Bool CheckEvent(::Display*, XEvent* event, XPointer userData);

Event ToStraitXEvent(const XEvent &event);


} //namespace Linux::
} //namespace StraitX::

#endif // STRAITX_LINUX_EVENTS_HPP