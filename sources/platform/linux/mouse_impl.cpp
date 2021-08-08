#include <X11/Xlib.h>
#undef Success
#undef None
#undef KeyPress
#undef KeyRelease
#undef FocusIn
#undef FocusOut
#include "core/os/mouse.hpp"
#include "core/os/window.hpp"
#include "platform/linux/display_server.hpp"

namespace Mouse{

using namespace Linux;	

bool IsButtonPressed(Mouse::Button button){
    ::Window root,child;
    int x,y;
    unsigned int button_mask;
    XQueryPointer(DisplayServer::Handle, RootWindow(DisplayServer::Handle, DefaultScreen(DisplayServer::Handle)),&root,&child,&x,&y,&x,&y,&button_mask);

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

Point2i GlobalPosition(){
    ::Window root,child;
    Point2i choosen,global;
    unsigned int mask;
    
    XQueryPointer(DisplayServer::Handle, RootWindow(DisplayServer::Handle, DefaultScreen(DisplayServer::Handle)),&root,&child,&global.x,&global.y,&choosen.x,&choosen.y,&mask);

	global.y = PlatformWindow::Screen().Size.height - global.y;

    return global;
}

Point2i RelativePosition(){
    ::Window root,child;
    Point2i choosen,global;
    unsigned int mask;

    XQueryPointer(DisplayServer::Handle, WindowImpl::s_MainWindow.Handle,&root,&child,&global.x,&global.y,&choosen.x,&choosen.y,&mask);
    
	i32 window_height = (i32)WindowImpl::s_MainWindow.Size().height;

	choosen.y = window_height - choosen.y;
    return choosen;
}

void SetGlobalPosition(const Point2i &position){
    Point2i new_position;
    new_position.x = position.x;
    new_position.y = PlatformWindow::Screen().Size.height - position.y;

    XWarpPointer(DisplayServer::Handle, 0, RootWindow(DisplayServer::Handle, DefaultScreen(DisplayServer::Handle)),0,0,0,0,new_position.x, new_position.y);
}

static Cursor BlankCursor(){
    static bool is_created;
    static Cursor cursor;

    if(!is_created){
        is_created = true;

        char data[1] = {0};
        Pixmap blank;
        XColor dummy;

        blank = XCreateBitmapFromData(DisplayServer::Handle, RootWindow(DisplayServer::Handle, DefaultScreen(DisplayServer::Handle)), data, 1, 1);
        if(blank == 0)
            is_created = false;
        cursor = XCreatePixmapCursor(DisplayServer::Handle, blank, blank, &dummy, &dummy, 0, 0);
        XFreePixmap(DisplayServer::Handle, blank);
    }
    return cursor;
}

void SetVisible(bool is_visible){
    if(is_visible){
        XUndefineCursor(DisplayServer::Handle, RootWindow(DisplayServer::Handle, DefaultScreen(DisplayServer::Handle)));
    }else{
        XDefineCursor(DisplayServer::Handle, RootWindow(DisplayServer::Handle, DefaultScreen(DisplayServer::Handle)), BlankCursor());
    }
}

}//namespace Mouse::