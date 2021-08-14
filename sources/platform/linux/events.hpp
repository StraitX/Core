#ifndef STRAITX_LINUX_EVENTS_HPP
#define STRAITX_LINUX_EVENTS_HPP

#include "core/os/events.hpp"
#include "platform/linux/x11.hpp"

namespace Linux{

struct WindowImpl;

Bool CheckEvent(X11::Display*, X11::XEvent* event, X11::XPointer userData);

bool ToStraitXEvent(X11::XEvent &in_event, Event &out_event, WindowImpl *window);

}//namespace Linux::

#endif // STRAITX_LINUX_EVENTS_HPP