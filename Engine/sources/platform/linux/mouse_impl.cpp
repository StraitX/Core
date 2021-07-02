#include <X11/Xlib.h>
#undef Success
#undef None
#undef KeyPress
#undef KeyRelease
#include "platform/mouse.hpp"
#include "platform/window.hpp"
#include "platform/window_system.hpp"

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

	global.y = WindowSystem::MainScreen().Size().height - global.y;

    return global;
}

Point Mouse::RelativePosition(const Window &window){
    ::Window root,child;
    Point choosen,global;
    unsigned int mask;

	s32 window_height = (s32)window.Size().height;
    
    XQueryPointer(Linux::s_Display,window.Impl().Handle,&root,&child,&global.x,&global.y,&choosen.x,&choosen.y,&mask);

	choosen.y = window_height - choosen.y;
    return choosen;
}

void Mouse::SetGlobalPosition(const Point &position){
    XWarpPointer(Linux::s_Display, 0, RootWindow(Linux::s_Display, DefaultScreen(Linux::s_Display)),0,0,0,0,position.x, position.y);
}

static Cursor BlankCursor(){
    static bool is_created;
    static Cursor cursor;

    if(!is_created){
        is_created = true;

        char data[1] = {0};
        Pixmap blank;
        XColor dummy;

        blank = XCreateBitmapFromData(Linux::s_Display, RootWindow(Linux::s_Display, DefaultScreen(Linux::s_Display)), data, 1, 1);
        if(blank == 0)
            is_created = false;
        cursor = XCreatePixmapCursor(Linux::s_Display, blank, blank, &dummy, &dummy, 0, 0);
        XFreePixmap(Linux::s_Display, blank);
    }
    return cursor;
}

void Mouse::SetVisible(bool is_visible){
    if(is_visible){
        XUndefineCursor(Linux::s_Display, RootWindow(Linux::s_Display, DefaultScreen(Linux::s_Display)));
    }else{
        XDefineCursor(Linux::s_Display, RootWindow(Linux::s_Display, DefaultScreen(Linux::s_Display)), BlankCursor());
    }
}

}//namespace StraitX::