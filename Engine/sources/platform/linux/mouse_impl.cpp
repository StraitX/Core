#include <X11/Xlib.h>
#undef Success
#undef None
#undef KeyPress
#undef KeyRelease
#include "platform/mouse.hpp"
#include "platform/window.hpp"

namespace StraitX{
namespace Linux{
extern ::Display *s_Display;
}//namespace Linux::

bool Mouse::IsButtonPressed(Mouse::Button button){
    ::Window root,child;
    int x,y;
    unsigned int button_mask;
    XQueryPointer(Linux::s_Display,RootWindow(Linux::s_Display,DefaultScreen(Linux::s_Display)),&root,&child,&x,&y,&x,&y,&button_mask);

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
    ::Window root,child;
    Point choosen,global;
    unsigned int mask;
    
    XQueryPointer(Linux::s_Display,RootWindow(Linux::s_Display,DefaultScreen(Linux::s_Display)),&root,&child,&global.x,&global.y,&choosen.x,&choosen.y,&mask);

    return global;
}

Point Mouse::RelativePosition(const Window &window){
    ::Window root,child;
    Point choosen,global;
    unsigned int mask;
    
    XQueryPointer(Linux::s_Display,window.Impl().Handle,&root,&child,&global.x,&global.y,&choosen.x,&choosen.y,&mask);
    return choosen;
}

}//namespace StraitX::