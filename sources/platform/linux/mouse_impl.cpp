#include "core/os/mouse.hpp"
#include "core/os/window.hpp"
#include "platform/linux/display_server.hpp"
#include "platform/linux/x11.hpp"

namespace Mouse{

using namespace Linux;	

bool IsButtonPressed(Mouse::Button button){
    X11::Window root,child;
    int x,y;
    unsigned int button_mask;
    X11::XQueryPointer(DisplayServer::Handle, X11::XRootWindow(DisplayServer::Handle, X11::XDefaultScreen(DisplayServer::Handle)),&root,&child,&x,&y,&x,&y,&button_mask);

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

Vector2s GlobalPosition(){
    X11::Window root,child;
    Vector2s choosen,global;
    unsigned int mask;
    
    X11::XQueryPointer(DisplayServer::Handle, X11::XRootWindow(DisplayServer::Handle, X11::XDefaultScreen(DisplayServer::Handle)),&root,&child,&global.x,&global.y,&choosen.x,&choosen.y,&mask);

    return global;
}

Vector2s RelativePosition(const Window &window){
    X11::Window root,child;
    Vector2s choosen,global;
    unsigned int mask;

    X11::XQueryPointer(DisplayServer::Handle, window.Impl().Handle() ,&root,&child,&global.x,&global.y,&choosen.x,&choosen.y,&mask);
    
    return choosen;
}

void SetGlobalPosition(const Vector2s &position){
    X11::XWarpPointer(DisplayServer::Handle, 0, X11::XRootWindow(DisplayServer::Handle, X11::XDefaultScreen(DisplayServer::Handle)),0,0,0,0,position.x, position.y);
}

static X11::Cursor BlankCursor(){
    static bool is_created;
    static X11::Cursor cursor;

    if(!is_created){
        is_created = true;

        char data[1] = {0};
        X11::Pixmap blank;
        X11::XColor dummy;

        blank = X11::XCreateBitmapFromData(DisplayServer::Handle, X11::XRootWindow(DisplayServer::Handle, X11::XDefaultScreen(DisplayServer::Handle)), data, 1, 1);
        if(blank == 0)
            is_created = false;
        cursor = X11::XCreatePixmapCursor(DisplayServer::Handle, blank, blank, &dummy, &dummy, 0, 0);
        X11::XFreePixmap(DisplayServer::Handle, blank);
    }
    return cursor;
}

void SetVisible(bool is_visible){
    if(is_visible){
        X11::XUndefineCursor(DisplayServer::Handle, X11::XRootWindow(DisplayServer::Handle, X11::XDefaultScreen(DisplayServer::Handle)));
    }else{
        X11::XDefineCursor(DisplayServer::Handle, X11::XRootWindow(DisplayServer::Handle, X11::XDefaultScreen(DisplayServer::Handle)), BlankCursor());
    }
}

}//namespace Mouse::