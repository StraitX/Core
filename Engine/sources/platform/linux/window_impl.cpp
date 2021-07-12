#include "platform/linux/window_impl.hpp"
#include "platform/linux/events.hpp"
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xatom.h>
#include <GL/glx.h>
#undef Success

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

Result WindowImpl::Open(const ScreenImpl &screen, int width, int height){
	Width = width;
	Height = height;

    FBConfig = PickBestFBConfig(screen.Index);
    if(FBConfig == nullptr)
        return Result::Unsupported;

    XVisualInfo *visualInfo = glXGetVisualFromFBConfig(s_Display, (GLXFBConfig)FBConfig);

    if(visualInfo == nullptr)
        return Result::Failure;
    
    XSetWindowAttributes attributes;
    attributes.background_pixel = BlackPixel(s_Display, screen.Index);
    attributes.colormap = XCreateColormap(s_Display, RootWindow(s_Display,screen.Index), visualInfo->visual, AllocNone);
    attributes.event_mask 	= ExposureMask 
							| KeyPressMask 
							| KeyReleaseMask 
							| ButtonPressMask 
							| ButtonReleaseMask 
							| StructureNotifyMask
							| FocusChangeMask;

    Handle = XCreateWindow(s_Display, RootWindow(s_Display,screen.Index), 0, 0, width, height, 0, visualInfo->depth,
        InputOutput, visualInfo->visual, CWBackPixel | CWColormap | CWEventMask, &attributes);

    XFree(visualInfo);

    if(Handle == 0)
        return Result::Failure;
    
    //intercept close event
    Atom close_atom = XInternAtom(s_Display,"WM_DELETE_WINDOW",0);
    XSetWMProtocols(s_Display,Handle,&close_atom,1);


    XMapWindow(s_Display,Handle);

	InputMethod = XOpenIM(s_Display, nullptr, nullptr, nullptr);

	if(!InputMethod){
		Close();
		return Result::Failure;
	}

	InputContext = XCreateIC(InputMethod,
                        XNInputStyle,   XIMPreeditNothing | XIMStatusNothing,
                        XNClientWindow, Handle,
                        XNFocusWindow,  Handle,
                        NULL);

	XSetICFocus(InputContext);

    return Result::Success;
}



Result WindowImpl::Close(){
	XDestroyIC(InputContext);
	
	XCloseIM(InputMethod);

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
	XFlush(s_Display);
}

Size2u WindowImpl::Size()const{
	return GetSizeFromHandle(Handle);
}

Size2u WindowImpl::GetSizeFromHandle(unsigned long handle){
    XWindowAttributes attributes;
    XGetWindowAttributes(s_Display, handle, &attributes);

    return {(u32)attributes.width, (u32)attributes.height};
}

__GLXFBConfigRec *WindowImpl::PickBestFBConfig(int screen_index){
    GLXFBConfig result = nullptr;

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


    result = configs[bestIndex];

    XFree(configs);

    return result;
}


}//namespace Linux::