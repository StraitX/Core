#ifndef STRAITX_WINDOWS_EVENTS_HPP
#define STRAITX_WINDOWS_EVENTS_HPP

#include "platform/events.hpp"
#include <windows.h>

namespace Windows{

bool ToStraitXEvent(MSG& message, Event &event);

}//namespace Windows::

#endif //STRAITX_WINDOWS_EVENTS_HPP