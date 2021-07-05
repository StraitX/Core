#ifndef STRAITX_LINUX_EVENTS_HPP
#define STRAITX_LINUX_EVENTS_HPP

#include "platform/events.hpp"
#include <X11/Xlib.h>

namespace Linux{

struct WindowImpl;

Bool CheckEvent(::Display*, XEvent* event, XPointer userData);

bool ToStraitXEvent(const XEvent &in_event, Event &out_event, WindowImpl *window);

}//namespace Linux::

#endif // STRAITX_LINUX_EVENTS_HPP