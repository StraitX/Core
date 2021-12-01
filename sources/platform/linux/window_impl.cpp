#include "platform/linux/window_impl.hpp"
#include "platform/linux/display_server.hpp"
#include "platform/linux/x11.hpp"
#include "platform/linux/keys.hpp"

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
	m_Width = width;
	m_Height = height;

	int screen_index = X11::XDefaultScreen(DisplayServer::Handle);

    m_FBConfig = PickBestFBConfig(screen_index);
    if(m_FBConfig == nullptr)
        return Result::Unsupported;

    X11::XVisualInfo *visualInfo = X11::glXGetVisualFromFBConfig(DisplayServer::Handle, (X11::GLXFBConfig)m_FBConfig);

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

    m_Handle = X11::XCreateWindow(DisplayServer::Handle, X11::XRootWindow(DisplayServer::Handle,screen_index), 0, 0, width, height, 0, visualInfo->depth,
        InputOutput, visualInfo->visual, CWBackPixel | CWColormap | CWEventMask, &attributes);

    X11::XFree(visualInfo);

    if(m_Handle == 0)
        return Result::Failure;
    
    //intercept close event
    X11::Atom close_atom = X11::XInternAtom(DisplayServer::Handle,"WM_DELETE_WINDOW",0);
    X11::XSetWMProtocols(DisplayServer::Handle,m_Handle,&close_atom,1);


    X11::XMapWindow(DisplayServer::Handle,m_Handle);

	m_InputMethod = X11::XOpenIM(DisplayServer::Handle, nullptr, nullptr, nullptr);

	if(!m_InputMethod){
		Close();
		return Result::Failure;
	}

	m_InputContext = X11::XCreateIC(m_InputMethod,
                        XNInputStyle,   XIMPreeditNothing | XIMStatusNothing,
                        XNClientWindow, m_Handle,
                        XNFocusWindow,  m_Handle,
                        NULL);

	X11::XSetICFocus(m_InputContext);

    SetTitle(title);

    return Result::Success;
}



Result WindowImpl::Close(){
	X11::XDestroyIC(m_InputContext);
	
	X11::XCloseIM(m_InputMethod);

    X11::XDestroyWindow(DisplayServer::Handle, m_Handle);

    m_Handle = 0;
    return Result::Success;
}

bool WindowImpl::IsOpen()const{
    return m_Handle != 0;
}

static Bool CheckEvent(X11::Display *display, X11::XEvent *event, X11::XPointer userData){
    return event->xany.window == reinterpret_cast<X11::Window>(userData);
}

void WindowImpl::SetEventsHandler(Function<void (const Event &)> handler){
	m_EventsHandler = handler;
}

void WindowImpl::DispatchEvents(){
	X11::XEvent in_event;

	while(XCheckIfEvent(DisplayServer::Handle, &in_event, &CheckEvent,reinterpret_cast<X11::XPointer>(m_Handle))){
		switch (in_event.type)
		{
		// Event recieved on window Close button 
		case X11::ClientMessage:
		{
			Event e;
			X11::Atom close_atom = X11::XInternAtom(in_event.xany.display,"WM_DELETE_WINDOW",0);
			if (in_event.xclient.data.l[0] == close_atom) {
				e.Type = EventType::WindowClose;
				m_EventsHandler.TryCall(e);
			}
		}break;
		case X11::ConfigureNotify:
		{
			Event e;
			if(in_event.xconfigure.width != m_Width || in_event.xconfigure.height != m_Height){
				e.Type = EventType::WindowResized;
				e.WindowResized.x = in_event.xconfigure.width;
				e.WindowResized.y = in_event.xconfigure.height;

				m_EventsHandler.TryCall(e);
			}

		}break;
		case X11::Expose:
		{
			Event e;
			e.Type = EventType::WindowDraw;
			m_EventsHandler.TryCall(e);
		}break;
		case X11::KeyPress:
		{
			int count = 0;
			X11::KeySym keysym = 0;
			char buf[20] = {};
			Status status = 0;
			count = X11::Xutf8LookupString(m_InputContext, (X11::XKeyPressedEvent*)&in_event, buf, 20, &keysym, &status);

			if (status==XBufferOverflow)
				break;

			if(status == XLookupChars || status == XLookupBoth){

				if(buf[0] <= 127){
					Event e;
					e.Type = EventType::TextEntered;
					e.TextEntered.Unicode = buf[0];
					m_EventsHandler.TryCall(e);
				}//TODO else handle utf-8 to utf-32 conversion
			}
			if(status == XLookupKeySym || status == XLookupBoth){
				Event e;
				e.Type = EventType::KeyPress;
				e.KeyPress.KeyCode = Key::Unknown;
				for(int i = 0; i<4; i++){
					e.KeyPress.KeyCode = XKeyCodeToKeyCode(X11::XLookupKeysym((X11::XKeyEvent*)&in_event.xkey,i));
					if(e.KeyPress.KeyCode != Key::Unknown)
						break;
				}
				m_EventsHandler.TryCall(e);
			}
		}break;
		case X11::KeyRelease:
		{
			Event e;
			e.Type = EventType::KeyRelease;
			e.KeyRelease.KeyCode = Key::Unknown;
			for(int i = 0; i<4; i++){
				e.KeyRelease.KeyCode = XKeyCodeToKeyCode(X11::XLookupKeysym((X11::XKeyEvent*)&in_event.xkey,i));
				if(e.KeyRelease.KeyCode != Key::Unknown)
					break;
			}
			m_EventsHandler.TryCall(e);
		}break;
		case X11::ButtonPress:
		{
			Event e;
			if(in_event.xbutton.button == Button4){
				e.Type = EventType::MouseWheel;
				e.MouseWheel.Delta = 1;
			}else if(in_event.xbutton.button == Button5){
				e.Type = EventType::MouseWheel;
				e.MouseWheel.Delta = -1;
			}else{
				e.Type = EventType::MouseButtonPress;
				e.MouseButtonPress.Button = XButtonToMouseButton(in_event.xbutton.button);
				e.MouseButtonPress.x = in_event.xbutton.x;
				e.MouseButtonPress.y = in_event.xbutton.y;
			}
			m_EventsHandler.TryCall(e);
		}break;
		case X11::ButtonRelease:
		{
			Event e;
			e.Type = EventType::MouseButtonRelease;
			e.MouseButtonRelease.Button = XButtonToMouseButton(in_event.xbutton.button);
			e.MouseButtonRelease.x = in_event.xbutton.x;
			e.MouseButtonRelease.y = in_event.xbutton.y;
			m_EventsHandler.TryCall(e);
		}break;
		case X11::FocusIn:
		{
			Event e;
			e.Type = EventType::FocusIn;
			m_EventsHandler.TryCall(e);
		}break;
		case X11::FocusOut:
		{
			Event e;
			e.Type = EventType::FocusOut;
			m_EventsHandler.TryCall(e);
		}break;
		default:
			(void)0;
		}
	}
}


void WindowImpl::SetTitle(const char *title){

    X11::Atom atom = X11::XInternAtom(DisplayServer::Handle,"WM_NAME",0);
    
    X11::XTextProperty titleText;
    char * pTitle = (char *)title;
    X11::XStringListToTextProperty(&pTitle,1,&titleText);
    X11::XSetTextProperty(DisplayServer::Handle,m_Handle,&titleText,atom);
}

void WindowImpl::SetSize(int width, int height){
    X11::XResizeWindow(DisplayServer::Handle,m_Handle,width,height);
	X11::XFlush(DisplayServer::Handle);
}

Vector2u WindowImpl::Size()const{
	X11::XWindowAttributes attributes;
    X11::XGetWindowAttributes(DisplayServer::Handle, m_Handle, &attributes);

    return {(u32)attributes.width, (u32)attributes.height};
}

const Screen &WindowImpl::CurrentScreen()const{
	// XXX: linux does not support multiple screens for now
	if(m_CurrentScreen.Handle == nullptr){
		m_CurrentScreen.Handle = X11::XDefaultScreenOfDisplay(DisplayServer::Handle);

		m_CurrentScreen.Size.x = X11::XWidthOfScreen((X11::Screen*)m_CurrentScreen.Handle);
		m_CurrentScreen.Size.y = X11::XHeightOfScreen((X11::Screen*)m_CurrentScreen.Handle);

		m_CurrentScreen.DPI.x  = float(m_CurrentScreen.Size.x) / (float(X11::XWidthMMOfScreen((X11::Screen*)m_CurrentScreen.Handle)) / 25.4f);
		m_CurrentScreen.DPI.y = float(m_CurrentScreen.Size.y) / (float(X11::XHeightMMOfScreen((X11::Screen*)m_CurrentScreen.Handle)) / 25.4f);
	}

	return m_CurrentScreen;
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