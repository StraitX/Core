#ifndef STRAITX_WINDOWS_EVENTS_HPP
#define STRAITX_WINDOWS_EVENTS_HPP

#include "platform/events.hpp"
#include <windows.h>

#define WM_SX_KEYDOWN	WM_USER + 1
#define WM_SX_KEYUP		WM_USER + 2
#define WM_SX_CLOSE		WM_USER + 3

namespace StraitX{
namespace Windows{

bool ToStraitXEvent(MSG& message, Event &event);

}; //namespace Windows::
}; //namespace StraitX::

#endif //STRAITX_WINDOWS_EVENTS_HPP