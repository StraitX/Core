#ifndef STRAITX_EVENTS_HPP
#define STRAITX_EVENTS_HPP

#include "core/env/compiler.hpp"
#include "core/os/keyboard.hpp"
#include "core/os/mouse.hpp"

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
    KeyRelease,
	TextEntered,
	FocusIn,
	FocusOut
};

struct WindowCloseEvent{ };

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

struct TextEnteredEvent{
	u32 Unicode;
};

struct FocusEvent{ };

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
		TextEnteredEvent TextEntered;
		FocusEvent FocusIn;
		FocusEvent FocusOut;
    };
};

#endif