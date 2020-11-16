#include "platform/linux/window_x11.hpp"
#include "platform/display.hpp"
#include "platform/linux/events_x11.hpp"
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xatom.h>
#include <GL/glx.h>
#undef Success

#include "stdio.h"
namespace StraitX{
namespace Linux{

WindowX11::WindowX11():
    mHandle(0)
{}

WindowX11::WindowX11(WindowX11 &&other){
    mHandle = other.mHandle;
    other.mHandle = 0;
}


Error WindowX11::Open(const ScreenX11 &screen, int width, int height, const FBConfigX11 &config){
    ::Display *display = Display::Instance().Impl().Handle();

    GLXFBConfig fbconfig = (GLXFBConfig)config.Handle();

    if(fbconfig==nullptr)
        return Error::InvalidArgs;

    XVisualInfo *visualInfo = glXGetVisualFromFBConfig(display, fbconfig);

    if(visualInfo == nullptr)
        return Error::Failure;
    
    XSetWindowAttributes attributes;
    attributes.background_pixel = BlackPixel(display, screen.m_Index);
    attributes.colormap = XCreateColormap(display, RootWindow(display,screen.m_Index), visualInfo->visual, AllocNone);
    attributes.event_mask = ExposureMask | KeyPressMask| KeyReleaseMask| ButtonPressMask| ButtonReleaseMask| ResizeRedirectMask;

    mHandle = XCreateWindow(display, RootWindow(display,screen.m_Index), 0, 0, width, height, 0, visualInfo->depth,
        InputOutput, visualInfo->visual, CWBackPixel | CWColormap | CWEventMask, &attributes);

    XFree(visualInfo);

    if(mHandle == 0)
        return Error::Failure;
    
    XMapWindow(display,mHandle);
    return Error::Success;
}



Error WindowX11::Close(){
    ::Display *display = Display::Instance().Impl().Handle();
    XDestroyWindow(display,mHandle);

    mHandle = 0;
    return Error::Success;
}

bool WindowX11::IsOpen()const{
    return mHandle;
}

void WindowX11::SetTitle(const char *title){
    ::Display *display = Display::Instance().Impl().Handle();

    Atom atom = XInternAtom(display,"WM_NAME",0);
    
    XTextProperty titleText;
    char * pTitle = (char *)title;
    XStringListToTextProperty(&pTitle,1,&titleText);
    XSetTextProperty(display,mHandle,&titleText,atom);
}

void WindowX11::SetSize(int width, int height){
    ::Display *display = Display::Instance().Impl().Handle();

    XResizeWindow(display,mHandle,width,height);
}

bool WindowX11::PollEvent(Event &event){
    ::Display *display = Display::Instance().Impl().Handle();
    XEvent x11event;
    if(XCheckIfEvent(display,&x11event,&CheckEvent,reinterpret_cast<XPointer>(mHandle))){
        event = ToStraitXEvent(x11event);
        return true;
    }
    return false;
}


}; //namespace Linux::
}; //namespace StraitX::