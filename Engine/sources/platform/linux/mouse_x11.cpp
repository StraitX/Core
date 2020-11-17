#include <X11/Xlib.h>
#undef Success
#undef None
#include "platform/mouse.hpp"
#include "platform/linux/display_x11.hpp"

namespace StraitX{

static Linux::DisplayX11 s_Display;

void Mouse::Initialize(const Linux::DisplayX11 &display){
    s_Display = display;
}

bool Mouse::IsButtonPressed(Mouse::Button button){
    ::Display *display = s_Display.Handle();

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

Point Mouse::GlobalPosition(){
    ::Display *display = s_Display.Handle();

    Window root,child;
    Point choosen,global;
    unsigned int mask;
    
    XQueryPointer(display,RootWindow(display,DefaultScreen(display)),&root,&child,&global.x,&global.y,&choosen.x,&choosen.y,&mask);

    return global;
}

};