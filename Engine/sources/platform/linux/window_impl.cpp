#include "platform/linux/window_impl.hpp"
#include "platform/linux/events.hpp"
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xatom.h>
#include <GL/glx.h>
#undef Success

namespace StraitX{
namespace Linux{

extern ::Display *s_Display;

struct PixelFormat{
    int Red = 0;
    int Green = 0;
    int Blue = 0;
    int Alpha = 0;
    int Depth = 0;
    int Stencil = 0;
    int Samples = 0;
};


WindowImpl::WindowImpl(WindowImpl &&other)
{
    Handle = other.Handle;
    other.Handle = 0;
    FBConfig = other.FBConfig;
}


Result WindowImpl::Open(const ScreenImpl &screen, int width, int height){

    FBConfig = PickBestFBConfig(screen.m_Index);
    if(FBConfig == nullptr)
        return Result::Unsupported;

    XVisualInfo *visualInfo = glXGetVisualFromFBConfig(s_Display, (GLXFBConfig)FBConfig);

    if(visualInfo == nullptr)
        return Result::Failure;
    
    XSetWindowAttributes attributes;
    attributes.background_pixel = BlackPixel(s_Display, screen.m_Index);
    attributes.colormap = XCreateColormap(s_Display, RootWindow(s_Display,screen.m_Index), visualInfo->visual, AllocNone);
    attributes.event_mask = ExposureMask | KeyPressMask| KeyReleaseMask| ButtonPressMask| ButtonReleaseMask| ResizeRedirectMask;

    Handle = XCreateWindow(s_Display, RootWindow(s_Display,screen.m_Index), 0, 0, width, height, 0, visualInfo->depth,
        InputOutput, visualInfo->visual, CWBackPixel | CWColormap | CWEventMask, &attributes);

    XFree(visualInfo);

    if(Handle == 0)
        return Result::Failure;
    
    //intercept close event
    Atom close_atom = XInternAtom(s_Display,"WM_DELETE_WINDOW",0);
    XSetWMProtocols(s_Display,Handle,&close_atom,1);


    XMapWindow(s_Display,Handle);

    return Result::Success;
}



Result WindowImpl::Close(){
    XDestroyWindow(s_Display,Handle);

    Handle = 0;
    return Result::Success;
}

bool WindowImpl::IsOpen()const{
    return Handle;
}

void WindowImpl::SetTitle(const char *title){

    Atom atom = XInternAtom(s_Display,"WM_NAME",0);
    
    XTextProperty titleText;
    char * pTitle = (char *)title;
    XStringListToTextProperty(&pTitle,1,&titleText);
    XSetTextProperty(s_Display,Handle,&titleText,atom);
}

void WindowImpl::SetSize(int width, int height){

    XResizeWindow(s_Display,Handle,width,height);
}

bool WindowImpl::PollEvent(Event &event){
    XEvent x11event;
    if(XCheckIfEvent(s_Display,&x11event,&CheckEvent,reinterpret_cast<XPointer>(Handle))){
        event = ToStraitXEvent(x11event);
        return true;
    }
    return false;
}


void *WindowImpl::PickBestFBConfig(int screen_index){
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
    GLXFBConfig *configs = glXChooseFBConfig(s_Display,screen_index,glxAttributes,&configsCount);

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
        glXGetFBConfigAttrib(s_Display, configs[i], GLX_RED_SIZE,     &current.Red);
        glXGetFBConfigAttrib(s_Display, configs[i], GLX_GREEN_SIZE,   &current.Green);
        glXGetFBConfigAttrib(s_Display, configs[i], GLX_BLUE_SIZE,    &current.Blue);
        glXGetFBConfigAttrib(s_Display, configs[i], GLX_ALPHA_SIZE,   &current.Alpha);
        glXGetFBConfigAttrib(s_Display, configs[i], GLX_DEPTH_SIZE,   &current.Depth);
        glXGetFBConfigAttrib(s_Display, configs[i], GLX_STENCIL_SIZE, &current.Stencil);
        glXGetFBConfigAttrib(s_Display, configs[i], GLX_SAMPLES,      &current.Samples);

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


} //namespace Linux::
} //namespace StraitX::