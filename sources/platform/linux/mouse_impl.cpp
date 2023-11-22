#include "core/os/mouse.hpp"
#include "core/os/window.hpp"
#include "platform/linux/display_server.hpp"
#include "platform/linux/x11.hpp"

namespace Mouse{

using namespace Linux;	

bool IsButtonPressed(Mouse::Button button){
    static DisplayServerClient s_DisplayServerClient;
    auto X11Handle = s_DisplayServerClient.GetX11ServerHandle();

    X11::Window root,child;
    int x,y;
    unsigned int button_mask;
    X11::XQueryPointer(X11Handle, X11::XRootWindow(X11Handle, X11::XDefaultScreen(X11Handle)),&root,&child,&x,&y,&x,&y,&button_mask);

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
    static DisplayServerClient s_DisplayServerClient;
    auto X11Handle = s_DisplayServerClient.GetX11ServerHandle();

    X11::Window root,child;
    Vector2s choosen,global;
    unsigned int mask;
    
    X11::XQueryPointer(X11Handle, X11::XRootWindow(X11Handle, X11::XDefaultScreen(X11Handle)),&root,&child,&global.x,&global.y,&choosen.x,&choosen.y,&mask);

    return global;
}

Vector2s RelativePosition(const Window &window){
    X11::Window root,child;
    Vector2s choosen,global;
    unsigned int mask;

    X11::XQueryPointer(window.Impl().GetX11ServerHandle(), window.Impl().Handle(), &root, &child, &global.x, &global.y, &choosen.x, &choosen.y, &mask);
    
    return choosen;
}

void SetGlobalPosition(const Vector2s &position){
    static DisplayServerClient s_DisplayServerClient;
    auto X11Handle = s_DisplayServerClient.GetX11ServerHandle();
    X11::XWarpPointer(X11Handle, 0, X11::XRootWindow(X11Handle, X11::XDefaultScreen(X11Handle)),0,0,0,0,position.x, position.y);
}

static X11::Cursor BlankCursor(){
    static DisplayServerClient s_DisplayServerClient;
    auto X11Handle = s_DisplayServerClient.GetX11ServerHandle();
    static bool is_created;
    static X11::Cursor cursor;

    if(!is_created){
        is_created = true;

        char data[1] = {0};
        X11::Pixmap blank;
        X11::XColor dummy;

        blank = X11::XCreateBitmapFromData(X11Handle, X11::XRootWindow(X11Handle, X11::XDefaultScreen(X11Handle)), data, 1, 1);
        if(blank == 0)
            is_created = false;
        cursor = X11::XCreatePixmapCursor(X11Handle, blank, blank, &dummy, &dummy, 0, 0);
        X11::XFreePixmap(X11Handle, blank);
    }
    return cursor;
}

void SetVisible(bool is_visible){
    static DisplayServerClient s_DisplayServerClient;
    auto X11Handle = s_DisplayServerClient.GetX11ServerHandle();
    if(is_visible){
        X11::XUndefineCursor(X11Handle, X11::XRootWindow(X11Handle, X11::XDefaultScreen(X11Handle)));
    }else{
        X11::XDefineCursor(X11Handle, X11::XRootWindow(X11Handle, X11::XDefaultScreen(X11Handle)), BlankCursor());
    }
}

}//namespace Mouse::