#include "platform/linux/window_x11.hpp"
#include "platform/linux/events_x11.hpp"
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xatom.h>
#include <GL/glx.h>
#undef Success

namespace StraitX{
namespace Linux{

WindowX11::WindowX11(DisplayX11 &display):
    m_Handle(0),
    m_Display(display)
{}

WindowX11::WindowX11(WindowX11 &&other):
    m_Display(other.m_Display)
{
    m_Handle = other.m_Handle;
    other.m_Handle = 0;
    m_FBConfig = other.m_FBConfig;
}


Error WindowX11::Open(const ScreenX11 &screen, int width, int height, const FBConfigX11 &config){
    ::Display *display = m_Display.Handle();

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

    m_Handle = XCreateWindow(display, RootWindow(display,screen.m_Index), 0, 0, width, height, 0, visualInfo->depth,
        InputOutput, visualInfo->visual, CWBackPixel | CWColormap | CWEventMask, &attributes);

    XFree(visualInfo);

    if(m_Handle == 0)
        return Error::Failure;
    
    XMapWindow(display,m_Handle);
    // do not forget to save window config after creation
    m_FBConfig = config;
    return Error::Success;
}



Error WindowX11::Close(){
    XDestroyWindow(m_Display.Handle(),m_Handle);

    m_Handle = 0;
    return Error::Success;
}

unsigned long WindowX11::Handle()const{
    return m_Handle;
}

DisplayX11 &WindowX11::Display(){
    return m_Display; 
}

const FBConfigX11 &WindowX11::FBConfig()const{
    return m_FBConfig;
}

bool WindowX11::IsOpen()const{
    return m_Handle;
}

void WindowX11::SetTitle(const char *title){
    ::Display *display = m_Display.Handle();

    Atom atom = XInternAtom(display,"WM_NAME",0);
    
    XTextProperty titleText;
    char * pTitle = (char *)title;
    XStringListToTextProperty(&pTitle,1,&titleText);
    XSetTextProperty(display,m_Handle,&titleText,atom);
}

void WindowX11::SetSize(int width, int height){

    XResizeWindow(m_Display.Handle(),m_Handle,width,height);
}

bool WindowX11::PollEvent(Event &event){
    XEvent x11event;
    if(XCheckIfEvent(m_Display.Handle(),&x11event,&CheckEvent,reinterpret_cast<XPointer>(m_Handle))){
        event = ToStraitXEvent(x11event);
        return true;
    }
    return false;
}


}; //namespace Linux::
}; //namespace StraitX::