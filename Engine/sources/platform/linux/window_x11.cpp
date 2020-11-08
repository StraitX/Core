#include "platform/linux/window_x11.hpp"
#include "platform/linux/display_x11.hpp"
#include "platform/linux/events_x11.hpp"
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xatom.h>

namespace StraitX{
namespace Linux{

WindowX11::WindowX11(int width, int height):
    mHandle(0),
    mScreenIndex(-1)
{
    ::Display *display = Linux::DisplayX11::Instance().Handle();
    mScreenIndex = DefaultScreen(display);

    mHandle = XCreateSimpleWindow(display,RootWindow(display,mScreenIndex),0,0,width,height,0,BlackPixel(display,mScreenIndex),WhitePixel(display,mScreenIndex));

    long mask = ExposureMask | KeyPressMask| KeyReleaseMask| ButtonPressMask| ButtonReleaseMask| ResizeRedirectMask;
    XSelectInput(display,mHandle,mask);
    XMapWindow(display,mHandle);
}

WindowX11::~WindowX11(){
    ::Display *display = Linux::DisplayX11::Instance().Handle();

    XDestroyWindow(display,mHandle);
}

void WindowX11::SetTitle(const char *title){
    ::Display *display = Linux::DisplayX11::Instance().Handle();

    Atom atom = XInternAtom(display,"WM_NAME",0);
    
    XTextProperty titleText;
    char * pTitle = (char *)title;
    XStringListToTextProperty(&pTitle,1,&titleText);
    XSetTextProperty(display,mHandle,&titleText,atom);
}

void WindowX11::SetSize(int width, int height){
    ::Display *display = Linux::DisplayX11::Instance().Handle();

    XResizeWindow(display,mHandle,width,height);
}

bool WindowX11::PollEvent(Event &event){
    ::Display *display = Linux::DisplayX11::Instance().Handle();
    XEvent x11event;
    if(XCheckIfEvent(display,&x11event,&CheckEvent,reinterpret_cast<XPointer>(mHandle))){
        event = ToStraitXEvent(x11event);
        return true;
    }else{
        return false;
    }
}


}; //namespace Linux::
}; //namespace StraitX::