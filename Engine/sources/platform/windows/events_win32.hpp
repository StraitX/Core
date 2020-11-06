#ifndef STRAITX_EVENTS_WIN32_HPP
#define STRAITX_EVENTS_WIN32_HPP

#include "platform/events.hpp"
#include <windows.h>

namespace StraitX{
namespace Windows{

bool ToStraitXEvent(MSG& message, Event &event);

}; //namespace Windows::
}; //namespace StraitX::

#endif // STRAITX_WINDOW_WIN32_HPP 