#include "platform/linux/window_impl.hpp"
#include "platform/linux/events.hpp"
#include "platform/linux/keys.hpp"
#include <X11/X.h>


namespace StraitX{
namespace Linux{

Bool CheckEvent(::Display *display, XEvent *event, XPointer userData){
    return event->xany.window == reinterpret_cast< ::Window >(userData);
}

Event ToStraitXEvent(const XEvent &event){
    Event sxEvent;
    Atom close_atom;
    switch (event.type)
    {
    // Event recieved on window Close button 
    case ClientMessage:
        close_atom = XInternAtom(event.xany.display,"WM_DELETE_WINDOW",0);
        if (event.xclient.data.l[0] == close_atom) {
            sxEvent.Type = EventType::WindowClose;
        }else{
            sxEvent.Type = EventType::Unknown;
        }
        return sxEvent;

    case ResizeRequest:
        sxEvent.Type = EventType::WindowResized;
        sxEvent.WindowResized.x = event.xresizerequest.width;
        sxEvent.WindowResized.y = event.xresizerequest.height;
        return sxEvent;

    case Expose:
        sxEvent.Type = EventType::WindowDraw;
        return sxEvent;
        
    case KeyPress:
        #undef KeyPress
        sxEvent.Type = EventType::KeyPress;
        sxEvent.KeyPress.KeyCode = Key::Unknown;
        for(int i = 0; i<4; i++){
            sxEvent.KeyPress.KeyCode = XKeyCodeToKeyCode(XLookupKeysym((XKeyEvent*)&event.xkey,i));
            if(sxEvent.KeyPress.KeyCode != Key::Unknown)
                break;
        }
        return sxEvent;  

    case KeyRelease:
        #undef KeyRelease
        sxEvent.Type = EventType::KeyRelease;
        sxEvent.KeyRelease.KeyCode = Key::Unknown;
        for(int i = 0; i<4; i++){
            sxEvent.KeyRelease.KeyCode = XKeyCodeToKeyCode(XLookupKeysym((XKeyEvent*)&event.xkey,i));
            if(sxEvent.KeyRelease.KeyCode != Key::Unknown)
                break;
        }
        return sxEvent;  

    case ButtonPress:
        if(event.xbutton.button == Button4){
            sxEvent.Type = EventType::MouseWheel;
            sxEvent.MouseWheel.Delta = 1;
        }else if(event.xbutton.button == Button5){
            sxEvent.Type = EventType::MouseWheel;
            sxEvent.MouseWheel.Delta = -1;
        }else{
            sxEvent.Type = EventType::MouseButtonPress;
            sxEvent.MouseButtonPress.Button = XButtonToMouseButton(event.xbutton.button);
            sxEvent.MouseButtonPress.x = event.xbutton.x;
            sxEvent.MouseButtonPress.y = WindowImpl::GetSizeFromHandle(event.xany.window).height - event.xbutton.y;
        }
        return sxEvent;

    case ButtonRelease:
        sxEvent.Type = EventType::MouseButtonRelease;
        sxEvent.MouseButtonRelease.Button = XButtonToMouseButton(event.xbutton.button);
        sxEvent.MouseButtonRelease.x = event.xbutton.x;
        sxEvent.MouseButtonRelease.y = WindowImpl::GetSizeFromHandle(event.xany.window).height - event.xbutton.y;
        return sxEvent;

    default:
        sxEvent.Type = EventType::Unknown;
        return sxEvent;
        break;
    }
}

} //namespace Linux::
} //namespace StraitX::