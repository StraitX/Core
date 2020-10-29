#include <X11/Xlib.h>
#undef Success
#include "platform/mouse.hpp"
#include "platform/linux/display.hpp"

namespace StraitX{

bool Mouse::IsButtonPressed(Mouse::Button button){
    Display *display = Linux::Display::Instance().Handle();

    Window root,child;
    int x,y;
    unsigned int button_mask;
    XQueryPointer(display,RootWindow(display,DefaultScreen(display)),&root,&child,&x,&y,&x,&y,&button_mask);

    switch (button)
    {
    case Mouse::Button::Left: return button_mask & Button1Mask;
    case Mouse::Button::Middle: return button_mask & Button2Mask;
    case Mouse::Button::Right: return button_mask & Button3Mask;
    case Mouse::Button::XButton1: return false;
    case Mouse::Button::XButton2: return false;
    default: return false;
    }
}

};