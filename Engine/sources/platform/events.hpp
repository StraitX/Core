#ifndef STRAITX_EVENTS_HPP
#define STRAITX_EVENTS_HPP

#include "platform/compiler.hpp"
#include "platform/keyboard.hpp"
#include "platform/mouse.hpp"
namespace StraitX{

enum class EventType{
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


struct WindowCloseEvent{
};

struct WindowResizedEvent{
    u32 x;
    u32 y;
};

struct WindowDrawEvent{
};

struct MouseButtonEvent{
    Mouse::Button Button;
    u32 x;
    u32 y;
};

struct KeyEvent{
    Keyboard::KeyCode KeyCode;
};


struct Event{
    EventType Type;
    union {
        WindowCloseEvent WindowClose;
        WindowResizedEvent WindowResized;
        WindowDrawEvent WindowDraw;
        MouseButtonEvent MouseButtonPress;
        MouseButtonEvent MouseButtonRelease;
        KeyEvent KeyPress;
        KeyEvent KeyRelease;
    };
};

}; // namespace StraitX::

#endif