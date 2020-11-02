#ifndef STRAITX_EVENTS_HPP
#define STRATIX_EVENTS_HPP

#include "platform/compiler.hpp"
#include "platform/keyboard.hpp"
#include "platform/mouse.hpp"
namespace StraitX{

enum EventType{
    Unknown = 0,
    WindowClose,
    WindowResize,
    MouseButtonPressed,
    KeyPressed
};

struct BaseEvent{
    EventType Type;
};

struct WindowCloseEvent: BaseEvent{
};

struct WindowResizeEvent: BaseEvent{
    u32 x;
    u32 y;
};

struct MouseButtonPressedEvent: BaseEvent{
    Mouse::Button Button;
    u32 x;
    u32 y;
};

struct KeyPressedEvent: BaseEvent{
    Keyboard::KeyCode KeyCode;
};


union Event{
    EventType Type;
    WindowCloseEvent WindowClose;
    WindowResizeEvent WindowResize;
    MouseButtonPressedEvent MouseButtonPressed;
    KeyPressedEvent KeyPressed;
};

}; // namespace StraitX::

#endif