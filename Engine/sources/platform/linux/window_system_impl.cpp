#include "platform/window_system.hpp"
#include <X11/Xlib.h>
#include <GL/glx.h>
#include <string.h>
#undef Success
#include "platform/linux/screen_impl.hpp"
#include "platform/types.hpp"
//undef X11 macro to resolve conflicts

#include "platform/result.hpp"


namespace StraitX{
namespace Linux{
::Display *s_Display = nullptr;

::Screen *screen = nullptr;
Size2i screen_size = {};
Size2f screen_dpi = {};

};// namespace Linux::

Result WindowSystem::Initialize(){
    Linux::s_Display = XOpenDisplay(nullptr);

	if(Linux::s_Display == nullptr)
		return Result::Failure;

	Linux::screen = DefaultScreenOfDisplay(Linux::s_Display);

    Linux::screen_size.width = XWidthOfScreen(Linux::screen);
    Linux::screen_size.height = XHeightOfScreen(Linux::screen);

    Linux::screen_dpi.width  = float(Linux::screen_size.width) / (float(XWidthMMOfScreen(Linux::screen)) / 25.4f);
    Linux::screen_dpi.height = float(Linux::screen_size.height) / (float(XHeightMMOfScreen(Linux::screen)) / 25.4f);

	return Result::Success;
}
Result WindowSystem::Finalize(){
    auto res = XCloseDisplay(Linux::s_Display);
    Linux::s_Display = 0;
    return ResultError(res != 0);
}

Screen WindowSystem::MainScreen(){
    return Screen(Linux::ScreenImpl(DefaultScreen(Linux::s_Display), Linux::screen, Linux::screen_size, Linux::screen_dpi));
}

}// namespace StraitX::