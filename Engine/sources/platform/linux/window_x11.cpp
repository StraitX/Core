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
    mHandle(0),
    mScreenIndex(-1)
{}

WindowX11::WindowX11(WindowX11 &&other){
    mHandle = other.mHandle;
    other.mHandle = 0;
    mScreenIndex = other.mScreenIndex;
    other.mScreenIndex = -1;
}


Error WindowX11::Open(int width, int height, const FBConfigX11 &config){
    if(!IsOpen()){
        ::Display *display = Display::Instance().Impl().Handle();
        mScreenIndex = DefaultScreen(display);


        GLXFBConfig fbconfig = (GLXFBConfig)config.Handle();

        if(fbconfig==nullptr)
            return Error::InvalidArgs;

        XVisualInfo *visualInfo = glXGetVisualFromFBConfig(display, fbconfig);

        if(visualInfo == nullptr)
            return Error::Failure;
        
        XSetWindowAttributes attributes;
        attributes.background_pixel = BlackPixel(display, mScreenIndex);
        attributes.colormap = XCreateColormap(display, RootWindow(display,mScreenIndex), visualInfo->visual, AllocNone);
        attributes.event_mask = ExposureMask | KeyPressMask| KeyReleaseMask| ButtonPressMask| ButtonReleaseMask| ResizeRedirectMask;

        mHandle = XCreateWindow(display, RootWindow(display,mScreenIndex), 0, 0, width, height, 0, visualInfo->depth,
            InputOutput, visualInfo->visual, CWBackPixel | CWColormap | CWEventMask, &attributes);

        XFree(visualInfo);

        if(mHandle == 0)
            return Error::Failure;
        
        XMapWindow(display,mHandle);
        return Error::Success;
                //long mask = ExposureMask | KeyPressMask| KeyReleaseMask| ButtonPressMask| ButtonReleaseMask| ResizeRedirectMask;
        //XSelectInput(display,mHandle,mask);
    }
    return Error::AlreadyDone;
}



Error WindowX11::Close(){
    if(IsOpen()){
        ::Display *display = Display::Instance().Impl().Handle();
        XDestroyWindow(display,mHandle);

        mHandle = 0;
        mScreenIndex = -1;
        return Error::Success;
    }
    return Error::NullObject;
}

bool WindowX11::IsOpen()const{
    return mHandle;
}

void WindowX11::SetTitle(const char *title){
    if(IsOpen()){
        ::Display *display = Display::Instance().Impl().Handle();

        Atom atom = XInternAtom(display,"WM_NAME",0);
        
        XTextProperty titleText;
        char * pTitle = (char *)title;
        XStringListToTextProperty(&pTitle,1,&titleText);
        XSetTextProperty(display,mHandle,&titleText,atom);
    }
}

void WindowX11::SetSize(int width, int height){
    if(IsOpen()){
        ::Display *display = Display::Instance().Impl().Handle();

        XResizeWindow(display,mHandle,width,height);
    }
}

bool WindowX11::PollEvent(Event &event){
    if(IsOpen()){
        ::Display *display = Display::Instance().Impl().Handle();
        XEvent x11event;
        if(XCheckIfEvent(display,&x11event,&CheckEvent,reinterpret_cast<XPointer>(mHandle))){
            event = ToStraitXEvent(x11event);
            return true;
        }
    }
    return false;
}


}; //namespace Linux::
}; //namespace StraitX::