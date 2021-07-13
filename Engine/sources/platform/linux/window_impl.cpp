#include "platform/linux/window_impl.hpp"
#include "platform/linux/events.hpp"
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xatom.h>
#include <GL/glx.h>
#include "platform/linux/display_server.hpp"

namespace Linux{

WindowImpl WindowImpl::s_MainWindow;

struct PixelFormat{
    int Red = 0;
    int Green = 0;
    int Blue = 0;
    int Alpha = 0;
    int Depth = 0;
    int Stencil = 0;
    int Samples = 0;
};

Result WindowImpl::Open(int width, int height){
	Width = width;
	Height = height;

	int screen_index = DefaultScreen(DisplayServer::Handle);

    FBConfig = PickBestFBConfig(screen_index);
    if(FBConfig == nullptr)
        return Result::Unsupported;

    XVisualInfo *visualInfo = glXGetVisualFromFBConfig(DisplayServer::Handle, (GLXFBConfig)FBConfig);

    if(visualInfo == nullptr)
        return Result::Failure;
    
    XSetWindowAttributes attributes;
    attributes.background_pixel = BlackPixel(DisplayServer::Handle, screen_index);
    attributes.colormap = XCreateColormap(DisplayServer::Handle, RootWindow(DisplayServer::Handle,screen_index), visualInfo->visual, AllocNone);
    attributes.event_mask 	= ExposureMask 
							| KeyPressMask 
							| KeyReleaseMask 
							| ButtonPressMask 
							| ButtonReleaseMask 
							| StructureNotifyMask
							| FocusChangeMask;

    Handle = XCreateWindow(DisplayServer::Handle, RootWindow(DisplayServer::Handle,screen_index), 0, 0, width, height, 0, visualInfo->depth,
        InputOutput, visualInfo->visual, CWBackPixel | CWColormap | CWEventMask, &attributes);

    XFree(visualInfo);

    if(Handle == 0)
        return Result::Failure;
    
    //intercept close event
    Atom close_atom = XInternAtom(DisplayServer::Handle,"WM_DELETE_WINDOW",0);
    XSetWMProtocols(DisplayServer::Handle,Handle,&close_atom,1);


    XMapWindow(DisplayServer::Handle,Handle);

	InputMethod = XOpenIM(DisplayServer::Handle, nullptr, nullptr, nullptr);

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

    XDestroyWindow(DisplayServer::Handle,Handle);

    Handle = 0;
    return Result::Success;
}

void WindowImpl::SetTitle(const char *title){

    Atom atom = XInternAtom(DisplayServer::Handle,"WM_NAME",0);
    
    XTextProperty titleText;
    char * pTitle = (char *)title;
    XStringListToTextProperty(&pTitle,1,&titleText);
    XSetTextProperty(DisplayServer::Handle,Handle,&titleText,atom);
}

void WindowImpl::SetSize(int width, int height){
    XResizeWindow(DisplayServer::Handle,Handle,width,height);
	XFlush(DisplayServer::Handle);
}

Size2u WindowImpl::Size()const{
	return GetSizeFromHandle(Handle);
}

const PlatformScreen &WindowImpl::Screen(){
	// XXX: linux does not support multiple screens for now
	if(CurrentScreen.Handle == nullptr){
		CurrentScreen.Handle = DefaultScreenOfDisplay(DisplayServer::Handle);

		CurrentScreen.Size.width = XWidthOfScreen((::Screen*)CurrentScreen.Handle);
		CurrentScreen.Size.height = XHeightOfScreen((::Screen*)CurrentScreen.Handle);

		CurrentScreen.DPI.width  = float(CurrentScreen.Size.width) / (float(XWidthMMOfScreen((::Screen*)CurrentScreen.Handle)) / 25.4f);
		CurrentScreen.DPI.height = float(CurrentScreen.Size.height) / (float(XHeightMMOfScreen((::Screen*)CurrentScreen.Handle)) / 25.4f);
	}

	return CurrentScreen;
}

Size2u WindowImpl::GetSizeFromHandle(unsigned long handle){
    XWindowAttributes attributes;
    XGetWindowAttributes(DisplayServer::Handle, handle, &attributes);

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
		0 // NULL Terminated arg array
    };

    int configsCount = 0;
    GLXFBConfig *configs = glXChooseFBConfig(DisplayServer::Handle,screen_index,glxAttributes,&configsCount);

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
        glXGetFBConfigAttrib(DisplayServer::Handle, configs[i], GLX_RED_SIZE,     &current.Red);
        glXGetFBConfigAttrib(DisplayServer::Handle, configs[i], GLX_GREEN_SIZE,   &current.Green);
        glXGetFBConfigAttrib(DisplayServer::Handle, configs[i], GLX_BLUE_SIZE,    &current.Blue);
        glXGetFBConfigAttrib(DisplayServer::Handle, configs[i], GLX_ALPHA_SIZE,   &current.Alpha);
        glXGetFBConfigAttrib(DisplayServer::Handle, configs[i], GLX_DEPTH_SIZE,   &current.Depth);
        glXGetFBConfigAttrib(DisplayServer::Handle, configs[i], GLX_STENCIL_SIZE, &current.Stencil);
        glXGetFBConfigAttrib(DisplayServer::Handle, configs[i], GLX_SAMPLES,      &current.Samples);

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