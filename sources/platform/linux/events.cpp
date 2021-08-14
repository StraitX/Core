#include "platform/linux/window_impl.hpp"
#include "platform/linux/events.hpp"
#include "platform/linux/keys.hpp"
#include "platform/linux/display_server.hpp"
#include "platform/linux/x11.hpp"

namespace Linux{


Bool CheckEvent(X11::Display *display, X11::XEvent *event, X11::XPointer userData){
    return event->xany.window == reinterpret_cast<X11::Window>(userData);
}

void PollEvents(const WindowImpl &window, void (*handler)(const Event &e)){
	X11::XEvent in_event;

	while(XCheckIfEvent(DisplayServer::Handle, &in_event, &CheckEvent,reinterpret_cast<X11::XPointer>(window.Handle))){
		switch (in_event.type)
		{
		// Event recieved on window Close button 
		case X11::ClientMessage:
		{
			Event e;
			X11::Atom close_atom = X11::XInternAtom(in_event.xany.display,"WM_DELETE_WINDOW",0);
			if (in_event.xclient.data.l[0] == close_atom) {
				e.Type = EventType::WindowClose;
				handler(e);
			}
		}break;
		case X11::ConfigureNotify:
		{
			Event e;
			if(in_event.xconfigure.width != window.Width || in_event.xconfigure.height != window.Height){
				e.Type = EventType::WindowResized;
				e.WindowResized.x = in_event.xconfigure.width;
				e.WindowResized.y = in_event.xconfigure.height;

				handler(e);
			}

		}break;
		case X11::Expose:
		{
			Event e;
			e.Type = EventType::WindowDraw;
			handler(e);
		}break;
		case X11::KeyPress:
		{
			int count = 0;
			X11::KeySym keysym = 0;
			char buf[20] = {};
			Status status = 0;
			count = X11::Xutf8LookupString(window.InputContext, (X11::XKeyPressedEvent*)&in_event, buf, 20, &keysym, &status);

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
					e.KeyPress.KeyCode = XKeyCodeToKeyCode(X11::XLookupKeysym((X11::XKeyEvent*)&in_event.xkey,i));
					if(e.KeyPress.KeyCode != Key::Unknown)
						break;
				}
				handler(e);
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
			handler(e);
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
				e.MouseButtonPress.y = WindowImpl::GetSizeFromHandle(in_event.xany.window).y - in_event.xbutton.y;
			}
			handler(e);
		}break;
		case X11::ButtonRelease:
		{
			Event e;
			e.Type = EventType::MouseButtonRelease;
			e.MouseButtonRelease.Button = XButtonToMouseButton(in_event.xbutton.button);
			e.MouseButtonRelease.x = in_event.xbutton.x;
			e.MouseButtonRelease.y = WindowImpl::GetSizeFromHandle(in_event.xany.window).y - in_event.xbutton.y;
			handler(e);
		}break;
		case X11::FocusIn:
		{
			Event e;
			e.Type = EventType::FocusIn;
			handler(e);
		}break;
		case X11::FocusOut:
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