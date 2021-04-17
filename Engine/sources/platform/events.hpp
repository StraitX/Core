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
    MouseWheel,
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

struct MouseWheelEvent {
    s32 Delta;
};

struct MouseButtonEvent{
    Mouse::Button Button;
    s32 x;
    s32 y;
};

struct KeyEvent{
    Key KeyCode;
};


struct Event{
    EventType Type;
    union {
        WindowCloseEvent WindowClose;
        WindowResizedEvent WindowResized;
        WindowDrawEvent WindowDraw;
        MouseWheelEvent MouseWheel;
        MouseButtonEvent MouseButtonPress;
        MouseButtonEvent MouseButtonRelease;
        KeyEvent KeyPress;
        KeyEvent KeyRelease;
    };
};

}; // namespace StraitX::

#endif