#include "platform/linux/window_x11.hpp"
#include "platform/linux/display.hpp"
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xatom.h>

namespace StraitX{
namespace Linux{

WindowX11::WindowX11(int width, int height):
    mHandle(0),
    mScreenIndex(-1)
{
    ::Display *display = Linux::Display::Instance().Handle();
    mScreenIndex = DefaultScreen(display);

    mHandle = XCreateSimpleWindow(display,RootWindow(display,mScreenIndex),0,0,width,height,0,BlackPixel(display,mScreenIndex),WhitePixel(display,mScreenIndex));

    XMapWindow(display,mHandle);
}

WindowX11::~WindowX11(){
    ::Display *display = Linux::Display::Instance().Handle();

    XDestroyWindow(display,mHandle);
    XEvent e;
    XNextEvent(display,&e);
}

void WindowX11::SetTitle(const char *title){
    ::Display *display = Linux::Display::Instance().Handle();

    Atom atom = XInternAtom(display,"WM_NAME",0);
    
    XTextProperty titleText;
    char * pTitle = (char *)title;
    XStringListToTextProperty(&pTitle,1,&titleText);
    XSetTextProperty(display,mHandle,&titleText,atom);
}

}; //namespace Linux::
}; //namespace StraitX::