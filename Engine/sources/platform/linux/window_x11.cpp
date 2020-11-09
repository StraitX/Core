#include "platform/linux/window_x11.hpp"
#include "platform/linux/display_x11.hpp"
#include "platform/linux/events_x11.hpp"
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xatom.h>
#undef Success


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


Error WindowX11::Open(int width, int height, const char *title){
    if(!IsOpen()){
        ::Display *display = Linux::DisplayX11::Instance().Handle();
        mScreenIndex = DefaultScreen(display);

        mHandle = XCreateSimpleWindow(display,RootWindow(display,mScreenIndex),0,0,width,height,0,BlackPixel(display,mScreenIndex),WhitePixel(display,mScreenIndex));
        if(mHandle == 0)
            return ErrorCode::Failure;
        
        long mask = ExposureMask | KeyPressMask| KeyReleaseMask| ButtonPressMask| ButtonReleaseMask| ResizeRedirectMask;
        XSelectInput(display,mHandle,mask);
        XMapWindow(display,mHandle);

        SetTitle(title);

        return ErrorCode::Success;
    }
    return ErrorCode::AlreadyDone;
}



Error WindowX11::Close(){
    if(IsOpen()){
        ::Display *display = Linux::DisplayX11::Instance().Handle();
        XDestroyWindow(display,mHandle);

        mHandle = 0;
        mScreenIndex = -1;
        return ErrorCode::Success;
    }
    return ErrorCode::DoesNotExist;
}

bool WindowX11::IsOpen(){
    return mHandle;
}

void WindowX11::SetTitle(const char *title){
    if(IsOpen()){
        ::Display *display = Linux::DisplayX11::Instance().Handle();

        Atom atom = XInternAtom(display,"WM_NAME",0);
        
        XTextProperty titleText;
        char * pTitle = (char *)title;
        XStringListToTextProperty(&pTitle,1,&titleText);
        XSetTextProperty(display,mHandle,&titleText,atom);
    }
}

void WindowX11::SetSize(int width, int height){
    if(IsOpen()){
        ::Display *display = Linux::DisplayX11::Instance().Handle();

        XResizeWindow(display,mHandle,width,height);
    }
}

bool WindowX11::PollEvent(Event &event){
    if(IsOpen()){
        ::Display *display = Linux::DisplayX11::Instance().Handle();
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