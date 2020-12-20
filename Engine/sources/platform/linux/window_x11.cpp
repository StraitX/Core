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
    Handle(0),
    Display(display)
{}

WindowX11::WindowX11(WindowX11 &&other):
    Display(other.Display)
{
    Handle = other.Handle;
    other.Handle = 0;
    FBConfig = other.FBConfig;
}


Result WindowX11::Open(const ScreenX11 &screen, int width, int height){
    ::Display *display = Display.Handle();

    FBConfig = PickBestFBConfig(Display, screen);
    if(FBConfig == nullptr)
        return Result::Unsupported;

    XVisualInfo *visualInfo = glXGetVisualFromFBConfig(display, (GLXFBConfig)FBConfig);

    if(visualInfo == nullptr)
        return Result::Failure;
    
    XSetWindowAttributes attributes;
    attributes.background_pixel = BlackPixel(display, screen.m_Index);
    attributes.colormap = XCreateColormap(display, RootWindow(display,screen.m_Index), visualInfo->visual, AllocNone);
    attributes.event_mask = ExposureMask | KeyPressMask| KeyReleaseMask| ButtonPressMask| ButtonReleaseMask| ResizeRedirectMask;

    Handle = XCreateWindow(display, RootWindow(display,screen.m_Index), 0, 0, width, height, 0, visualInfo->depth,
        InputOutput, visualInfo->visual, CWBackPixel | CWColormap | CWEventMask, &attributes);

    XFree(visualInfo);

    if(Handle == 0)
        return Result::Failure;
    
    //intercept close event
    Atom close_atom = XInternAtom(display,"WM_DELETE_WINDOW",0);
    XSetWMProtocols(display,Handle,&close_atom,1);


    XMapWindow(display,Handle);

    return Result::Success;
}



Result WindowX11::Close(){
    XDestroyWindow(Display.Handle(),Handle);

    Handle = 0;
    return Result::Success;
}

bool WindowX11::IsOpen()const{
    return Handle;
}

void WindowX11::SetTitle(const char *title){
    ::Display *display = Display.Handle();

    Atom atom = XInternAtom(display,"WM_NAME",0);
    
    XTextProperty titleText;
    char * pTitle = (char *)title;
    XStringListToTextProperty(&pTitle,1,&titleText);
    XSetTextProperty(display,Handle,&titleText,atom);
}

void WindowX11::SetSize(int width, int height){

    XResizeWindow(Display.Handle(),Handle,width,height);
}

bool WindowX11::PollEvent(Event &event){
    XEvent x11event;
    if(XCheckIfEvent(Display.Handle(),&x11event,&CheckEvent,reinterpret_cast<XPointer>(Handle))){
        event = ToStraitXEvent(x11event);
        return true;
    }
    return false;
}


void *WindowX11::PickBestFBConfig(const DisplayX11 &display_x11, const ScreenX11 &screen){
    ::Display *display = display_x11.Handle();
    void *result = nullptr;

    // if client system doesn't have such a FBConfig, we are going to drop it
    int glxAttributes[]={
		GLX_X_RENDERABLE    , True,
		GLX_DRAWABLE_TYPE   , GLX_WINDOW_BIT,
		GLX_RENDER_TYPE     , GLX_RGBA_BIT,
		GLX_X_VISUAL_TYPE   , GLX_TRUE_COLOR,
		GLX_DOUBLEBUFFER    , True,
		None // NULL Terminated arg array
    };

    int configsCount = 0;
    GLXFBConfig *configs = glXChooseFBConfig(display,screen.m_Index,glxAttributes,&configsCount);

    // in case if we have not find any suitable FBConfig
    if(configsCount == 0)
        return nullptr;

    int bestIndex = 0;
    float bestScore = 0;

    constexpr PixelFormat desired = {
        .Red = 8,
        .Green = 8,
        .Blue = 8,
        .Alpha = 8,
        .Depth = 24,
        .Stencil = 8,
        .Samples = 1
    };

    for(int i = 0; i<configsCount; i++){
        PixelFormat current;
        glXGetFBConfigAttrib(display, configs[i], GLX_RED_SIZE,     &current.Red);
        glXGetFBConfigAttrib(display, configs[i], GLX_GREEN_SIZE,   &current.Green);
        glXGetFBConfigAttrib(display, configs[i], GLX_BLUE_SIZE,    &current.Blue);
        glXGetFBConfigAttrib(display, configs[i], GLX_ALPHA_SIZE,   &current.Alpha);
        glXGetFBConfigAttrib(display, configs[i], GLX_DEPTH_SIZE,   &current.Depth);
        glXGetFBConfigAttrib(display, configs[i], GLX_STENCIL_SIZE, &current.Stencil);
        glXGetFBConfigAttrib(display, configs[i], GLX_SAMPLES,      &current.Samples);

        float score = float(current.Red)/float(desired.Red) + float(current.Green)/float(desired.Green) + float(current.Blue)/float(desired.Blue) + 
                float(current.Alpha)/float(desired.Alpha) + float(current.Depth)/float(desired.Depth) + float(current.Stencil)/float(desired.Stencil) + float(current.Samples)/float(desired.Samples) - 7;
        
        if(score > bestScore){
            bestScore = score;
            bestIndex = i;
        }
    }


    result = (void*)configs[bestIndex];

    XFree(configs);

    return result;
}


}; //namespace Linux::
}; //namespace StraitX::