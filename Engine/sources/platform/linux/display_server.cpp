#include "platform/linux/display_server.hpp"
#include <X11/Xlib.h>

namespace Linux{

::Display *DisplayServer::Handle = nullptr;

Result DisplayServer::Open(){
	Handle = XOpenDisplay(nullptr);
	return Result(IsOpen());
}

void DisplayServer::Close(){
	XCloseDisplay(Handle);
	Handle = nullptr;
}

bool DisplayServer::IsOpen(){
	return Handle != nullptr;
}

}//namespace Linux::