#include "platform/linux/display_server.hpp"

namespace Linux{

X11::Display *DisplayServer::Handle = nullptr;

Result DisplayServer::Open(){
	Handle = X11::XOpenDisplay(nullptr);
	return Result(IsOpen());
}

void DisplayServer::Close(){
	X11::XCloseDisplay(Handle);
	Handle = nullptr;
}

bool DisplayServer::IsOpen(){
	return Handle != nullptr;
}

}//namespace Linux::