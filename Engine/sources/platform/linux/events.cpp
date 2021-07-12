#include "platform/linux/window_impl.hpp"
#include "platform/linux/events.hpp"
#include "platform/linux/keys.hpp"
#include <X11/X.h>

namespace Linux{

extern ::Display *s_Display;

Bool CheckEvent(::Display *display, XEvent *event, XPointer userData){
    return event->xany.window == reinterpret_cast< ::Window >(userData);
}

void PollEvents(const WindowImpl &window, void (*handler)(const Event &e)){
	XEvent in_event;

	while(XCheckIfEvent(s_Display, &in_event, &CheckEvent,reinterpret_cast<XPointer>(window.Handle))){
		switch (in_event.type)
		{
		// Event recieved on window Close button 
		case ClientMessage:
		{
			Event e;
			Atom close_atom = XInternAtom(in_event.xany.display,"WM_DELETE_WINDOW",0);
			if (in_event.xclient.data.l[0] == close_atom) {
				e.Type = EventType::WindowClose;
				handler(e);
			}
		}break;
		case ConfigureNotify:
		{
			Event e;
			if(in_event.xconfigure.width != window.Width || in_event.xconfigure.height != window.Height){
				e.Type = EventType::WindowResized;
				e.WindowResized.x = in_event.xconfigure.width;
				e.WindowResized.y = in_event.xconfigure.height;

				handler(e);
			}

		}break;
		case Expose:
		{
			Event e;
			e.Type = EventType::WindowDraw;
			handler(e);
		}break;
		case KeyPress:
		#undef KeyPress
		{
			int count = 0;
			KeySym keysym = 0;
			char buf[20] = {};
			Status status = 0;
			count = Xutf8LookupString(window.InputContext, (XKeyPressedEvent*)&in_event, buf, 20, &keysym, &status);

			if (status==XBufferOverflow)
				break;

			if(status == XLookupChars || status == XLookupBoth){

				if(buf[0] <= 127){
					Event e;
					e.Type = EventType::TextEntered;
					e.TextEntered.Unicode = buf[0];
					handler(e);
				}//TODO else handle utf-8 to utf-32 conversion
			}
			if(status == XLookupKeySym || status == XLookupBoth){
				Event e;
				e.Type = EventType::KeyPress;
				e.KeyPress.KeyCode = Key::Unknown;
				for(int i = 0; i<4; i++){
					e.KeyPress.KeyCode = XKeyCodeToKeyCode(XLookupKeysym((XKeyEvent*)&in_event.xkey,i));
					if(e.KeyPress.KeyCode != Key::Unknown)
						break;
				}
				handler(e);
			}
		}break;
		case KeyRelease:
		#undef KeyRelease
		{
			Event e;
			e.Type = EventType::KeyRelease;
			e.KeyRelease.KeyCode = Key::Unknown;
			for(int i = 0; i<4; i++){
				e.KeyRelease.KeyCode = XKeyCodeToKeyCode(XLookupKeysym((XKeyEvent*)&in_event.xkey,i));
				if(e.KeyRelease.KeyCode != Key::Unknown)
					break;
			}
			handler(e);
		}break;
		case ButtonPress:
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
				e.MouseButtonPress.y = WindowImpl::GetSizeFromHandle(in_event.xany.window).height - in_event.xbutton.y;
			}
			handler(e);
		}break;
		case ButtonRelease:
		{
			Event e;
			e.Type = EventType::MouseButtonRelease;
			e.MouseButtonRelease.Button = XButtonToMouseButton(in_event.xbutton.button);
			e.MouseButtonRelease.x = in_event.xbutton.x;
			e.MouseButtonRelease.y = WindowImpl::GetSizeFromHandle(in_event.xany.window).height - in_event.xbutton.y;
			handler(e);
		}break;
		case FocusIn:
		#undef FocusIn
		{
			Event e;
			e.Type = EventType::FocusIn;
			handler(e);
		}break;
		case FocusOut:
		#undef FocusOut
		{
			Event e;
			e.Type = EventType::FocusOut;
			handler(e);
		}break;
		default:
			(void)0;
		}
	}
}

}//namespace Linux::