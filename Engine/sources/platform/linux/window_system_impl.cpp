#include "platform/window_system.hpp"
#include <X11/Xlib.h>
#include <GL/glx.h>
#include <string.h>
#undef Success
#include "platform/linux/screen_impl.hpp"
#include "platform/types.hpp"
//undef X11 macro to resolve conflicts

#include "platform/result.hpp"

namespace Linux{
::Display *s_Display = nullptr;
ScreenImpl s_Screen;
WindowImpl s_Window;
}//namespace Linux::

Result WindowSystem::Initialize(int width, int height){
	using namespace Linux;

    s_Display = XOpenDisplay(nullptr);

	if(s_Display == nullptr)
		return Result::Failure;

	s_Screen = ScreenImpl(s_Display);


	return s_Window.Open(s_Screen, width, height);
}
void WindowSystem::Finalize(){
	using namespace Linux;

	s_Window.Close();

    XCloseDisplay(s_Display);
}

PlatformScreen WindowSystem::MainScreen(){
    return PlatformScreen(Linux::s_Screen);
}

PlatformWindow WindowSystem::Window(){
	return PlatformWindow(Linux::s_Window);
}