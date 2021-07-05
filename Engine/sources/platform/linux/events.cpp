#include "platform/linux/window_impl.hpp"
#include "platform/linux/events.hpp"
#include "platform/linux/keys.hpp"
#include <X11/X.h>

namespace Linux{

Bool CheckEvent(::Display *display, XEvent *event, XPointer userData){
    return event->xany.window == reinterpret_cast< ::Window >(userData);
}

bool ToStraitXEvent(const XEvent &in_event, Event &out_event, WindowImpl *window){
    Atom close_atom;
    switch (in_event.type)
    {
    // Event recieved on window Close button 
    case ClientMessage:
        close_atom = XInternAtom(in_event.xany.display,"WM_DELETE_WINDOW",0);
        if (in_event.xclient.data.l[0] == close_atom) {
            out_event.Type = EventType::WindowClose;
			return true;
        }
        return false;

    case ConfigureNotify:
		if(in_event.xconfigure.width != window->Width || in_event.xconfigure.height != window->Height){
			out_event.Type = EventType::WindowResized;
			out_event.WindowResized.x = in_event.xconfigure.width;
			out_event.WindowResized.y = in_event.xconfigure.height;

			return true;
		}
        return false;

    case Expose:
        out_event.Type = EventType::WindowDraw;
        return true;
        
    case KeyPress:
        #undef KeyPress
        out_event.Type = EventType::KeyPress;
        out_event.KeyPress.KeyCode = Key::Unknown;
        for(int i = 0; i<4; i++){
            out_event.KeyPress.KeyCode = XKeyCodeToKeyCode(XLookupKeysym((XKeyEvent*)&in_event.xkey,i));
            if(out_event.KeyPress.KeyCode != Key::Unknown)
                break;
        }
        return true;  

    case KeyRelease:
        #undef KeyRelease
        out_event.Type = EventType::KeyRelease;
        out_event.KeyRelease.KeyCode = Key::Unknown;
        for(int i = 0; i<4; i++){
            out_event.KeyRelease.KeyCode = XKeyCodeToKeyCode(XLookupKeysym((XKeyEvent*)&in_event.xkey,i));
            if(out_event.KeyRelease.KeyCode != Key::Unknown)
                break;
        }
        return true;  

    case ButtonPress:
        if(in_event.xbutton.button == Button4){
            out_event.Type = EventType::MouseWheel;
            out_event.MouseWheel.Delta = 1;
        }else if(in_event.xbutton.button == Button5){
            out_event.Type = EventType::MouseWheel;
            out_event.MouseWheel.Delta = -1;
        }else{
            out_event.Type = EventType::MouseButtonPress;
            out_event.MouseButtonPress.Button = XButtonToMouseButton(in_event.xbutton.button);
            out_event.MouseButtonPress.x = in_event.xbutton.x;
            out_event.MouseButtonPress.y = WindowImpl::GetSizeFromHandle(in_event.xany.window).height - in_event.xbutton.y;
        }
        return true;

    case ButtonRelease:
        out_event.Type = EventType::MouseButtonRelease;
        out_event.MouseButtonRelease.Button = XButtonToMouseButton(in_event.xbutton.button);
        out_event.MouseButtonRelease.x = in_event.xbutton.x;
        out_event.MouseButtonRelease.y = WindowImpl::GetSizeFromHandle(in_event.xany.window).height - in_event.xbutton.y;
        return true;
    }
	return false;
}

}//namespace Linux::