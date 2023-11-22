#include <cassert>
#include "platform/linux/display_server.hpp"

namespace Linux{

static X11::_XDisplay* s_DisplayServerHandle = nullptr;
static size_t s_DisplayServerClientCount = 0;

DisplayServerClient::DisplayServerClient() {
	if (s_DisplayServerClientCount++ == 0){
		s_DisplayServerHandle = X11::XOpenDisplay(nullptr);
		X11::XInitThreads();
	}
}

DisplayServerClient::~DisplayServerClient() {
	if (s_DisplayServerClientCount-- == 0) {
		X11::XCloseDisplay(s_DisplayServerHandle);
		s_DisplayServerHandle = nullptr;
	}
}

X11::_XDisplay* DisplayServerClient::GetX11ServerHandle()const {
	assert(s_DisplayServerHandle);
	return s_DisplayServerHandle;
}

void DisplayServerClient::ForceInit() {
	static DisplayServerClient client;
}


}//namespace Linux::