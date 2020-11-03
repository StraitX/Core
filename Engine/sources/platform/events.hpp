#ifndef STRAITX_EVENTS_HPP
#define STRAITX_EVENTS_HPP

#include "platform/compiler.hpp"
#include "platform/keyboard.hpp"
#include "platform/mouse.hpp"
namespace StraitX{

enum EventType{
    Unknown = 0,
    WindowOpen,
    WindowClose,
    WindowResize,
    WindowDraw,
    MouseButtonPress,
    MouseButtonRelease,
    KeyPress,
    KeyRelease
};

struct BaseEvent{
    EventType Type;
};

struct WindowOpenEvent: BaseEvent{
};
struct WindowCloseEvent: BaseEvent{
};

struct WindowResizeEvent: BaseEvent{
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
    WindowOpenEvent WindowOpen;
    WindowResizeEvent WindowResize;
    WindowDrawEvent WindowDraw;
    MouseButtonEvent MouseButtonPress;
    MouseButtonEvent MouseButtonRelease;
    KeyEvent KeyPress;
    KeyEvent KeyRelease;
};

}; // namespace StraitX::

#endif