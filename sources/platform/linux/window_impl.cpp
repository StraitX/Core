#include "platform/linux/window_impl.hpp"
#include "platform/linux/events.hpp"
#include "platform/linux/display_server.hpp"
#include "platform/linux/x11.hpp"

namespace Linux{

struct PixelFormat{
    int Red = 0;
    int Green = 0;
    int Blue = 0;
    int Alpha = 0;
    int Depth = 0;
    int Stencil = 0;
    int Samples = 0;
};

Result WindowImpl::Open(int width, int height, const char *title){
	Width = width;
	Height = height;

	int screen_index = X11::XDefaultScreen(DisplayServer::Handle);

    FBConfig = PickBestFBConfig(screen_index);
    if(FBConfig == nullptr)
        return Result::Unsupported;

    X11::XVisualInfo *visualInfo = X11::glXGetVisualFromFBConfig(DisplayServer::Handle, (X11::GLXFBConfig)FBConfig);

    if(visualInfo == nullptr)
        return Result::Failure;
    
    X11::XSetWindowAttributes attributes;
    attributes.background_pixel = X11::XBlackPixel(DisplayServer::Handle, screen_index);
    attributes.colormap = X11::XCreateColormap(DisplayServer::Handle, X11::XRootWindow(DisplayServer::Handle,screen_index), visualInfo->visual, AllocNone);
    attributes.event_mask 	= ExposureMask 
							| KeyPressMask 
							| KeyReleaseMask 
							| ButtonPressMask 
							| ButtonReleaseMask 
							| StructureNotifyMask
							| FocusChangeMask;

    Handle = X11::XCreateWindow(DisplayServer::Handle, X11::XRootWindow(DisplayServer::Handle,screen_index), 0, 0, width, height, 0, visualInfo->depth,
        InputOutput, visualInfo->visual, CWBackPixel | CWColormap | CWEventMask, &attributes);

    X11::XFree(visualInfo);

    if(Handle == 0)
        return Result::Failure;
    
    //intercept close event
    X11::Atom close_atom = X11::XInternAtom(DisplayServer::Handle,"WM_DELETE_WINDOW",0);
    X11::XSetWMProtocols(DisplayServer::Handle,Handle,&close_atom,1);


    X11::XMapWindow(DisplayServer::Handle,Handle);

	InputMethod = X11::XOpenIM(DisplayServer::Handle, nullptr, nullptr, nullptr);

	if(!InputMethod){
		Close();
		return Result::Failure;
	}

	InputContext = X11::XCreateIC(InputMethod,
                        XNInputStyle,   XIMPreeditNothing | XIMStatusNothing,
                        XNClientWindow, Handle,
                        XNFocusWindow,  Handle,
                        NULL);

	X11::XSetICFocus(InputContext);

    SetTitle(title);

    return Result::Success;
}



Result WindowImpl::Close(){
	X11::XDestroyIC(InputContext);
	
	X11::XCloseIM(InputMethod);

    X11::XDestroyWindow(DisplayServer::Handle,Handle);

    Handle = 0;
    return Result::Success;
}

bool WindowImpl::IsOpen()const{
    return Handle != 0;
}

void WindowImpl::SetTitle(const char *title){

    X11::Atom atom = X11::XInternAtom(DisplayServer::Handle,"WM_NAME",0);
    
    X11::XTextProperty titleText;
    char * pTitle = (char *)title;
    X11::XStringListToTextProperty(&pTitle,1,&titleText);
    X11::XSetTextProperty(DisplayServer::Handle,Handle,&titleText,atom);
}

void WindowImpl::SetSize(int width, int height){
    X11::XResizeWindow(DisplayServer::Handle,Handle,width,height);
	X11::XFlush(DisplayServer::Handle);
}

Vector2u WindowImpl::Size()const{
	return GetSizeFromHandle(Handle);
}

const PlatformScreen &WindowImpl::Screen()const{
	// XXX: linux does not support multiple screens for now
	if(CurrentScreen.Handle == nullptr){
		CurrentScreen.Handle = X11::XDefaultScreenOfDisplay(DisplayServer::Handle);

		CurrentScreen.Size.x = X11::XWidthOfScreen((X11::Screen*)CurrentScreen.Handle);
		CurrentScreen.Size.y = X11::XHeightOfScreen((X11::Screen*)CurrentScreen.Handle);

		CurrentScreen.DPI.x  = float(CurrentScreen.Size.x) / (float(X11::XWidthMMOfScreen((X11::Screen*)CurrentScreen.Handle)) / 25.4f);
		CurrentScreen.DPI.y = float(CurrentScreen.Size.y) / (float(X11::XHeightMMOfScreen((X11::Screen*)CurrentScreen.Handle)) / 25.4f);
	}

	return CurrentScreen;
}

Vector2u WindowImpl::GetSizeFromHandle(unsigned long handle){
    X11::XWindowAttributes attributes;
    X11::XGetWindowAttributes(DisplayServer::Handle, handle, &attributes);

    return {(u32)attributes.width, (u32)attributes.height};
}

X11::__GLXFBConfigRec *WindowImpl::PickBestFBConfig(int screen_index){
    X11::GLXFBConfig result = nullptr;

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
    X11::GLXFBConfig *configs = X11::glXChooseFBConfig(DisplayServer::Handle,screen_index,glxAttributes,&configsCount);

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
        X11::glXGetFBConfigAttrib(DisplayServer::Handle, configs[i], GLX_RED_SIZE,     &current.Red);
        X11::glXGetFBConfigAttrib(DisplayServer::Handle, configs[i], GLX_GREEN_SIZE,   &current.Green);
        X11::glXGetFBConfigAttrib(DisplayServer::Handle, configs[i], GLX_BLUE_SIZE,    &current.Blue);
        X11::glXGetFBConfigAttrib(DisplayServer::Handle, configs[i], GLX_ALPHA_SIZE,   &current.Alpha);
        X11::glXGetFBConfigAttrib(DisplayServer::Handle, configs[i], GLX_DEPTH_SIZE,   &current.Depth);
        X11::glXGetFBConfigAttrib(DisplayServer::Handle, configs[i], GLX_STENCIL_SIZE, &current.Stencil);
        X11::glXGetFBConfigAttrib(DisplayServer::Handle, configs[i], GLX_SAMPLES,      &current.Samples);

        float score = float(current.Red)/float(desired.Red) + float(current.Green)/float(desired.Green) + float(current.Blue)/float(desired.Blue) + 
                float(current.Alpha)/float(desired.Alpha) + float(current.Depth)/float(desired.Depth) + float(current.Stencil)/float(desired.Stencil) + float(current.Samples)/float(desired.Samples) - 7;
        
        if(score > bestScore){
            bestScore = score;
            bestIndex = i;
        }
    }


    result = configs[bestIndex];

    X11::XFree(configs);

    return result;
}


}//namespace Linux::