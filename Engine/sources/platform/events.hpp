#ifndef STRAITX_EVENTS_HPP
#define STRAITX_EVENTS_HPP

#include "platform/compiler.hpp"
#include "platform/keyboard.hpp"
#include "platform/mouse.hpp"
namespace StraitX{

enum EventType{
    Unknown = 0,
    // posted when window close request arrives
    WindowClose,
    WindowResized,
    WindowDraw,
    MouseButtonPress,
    MouseButtonRelease,
    KeyPress,
    KeyRelease
};

struct BaseEvent{
    EventType Type;
};

struct WindowCloseEvent: BaseEvent{
};

struct WindowResizedEvent: BaseEvent{
    u32 x;
    u32 y;
};

struct WindowDrawEvent: BaseEvent{
};

struct MouseButtonEvent: BaseEvent{
    Mouse::Button Button;
    u32 x;
    u32 y;
};

struct KeyEvent: BaseEvent{
    Keyboard::KeyCode KeyCode;
};


union Event{
    EventType Type;
    WindowCloseEvent WindowClose;
    WindowResizedEvent WindowResized;
    WindowDrawEvent WindowDraw;
    MouseButtonEvent MouseButtonPress;
    MouseButtonEvent MouseButtonRelease;
    KeyEvent KeyPress;
    KeyEvent KeyRelease;
};

}; // namespace StraitX::

#endif