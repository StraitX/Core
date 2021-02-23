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
    
};// namespace Linux::

Result WindowSystem::Initialize(){
    Linux::s_Display = XOpenDisplay(nullptr);
    return ResultError(Linux::s_Display == nullptr);
}
Result WindowSystem::Finalize(){
    auto res = XCloseDisplay(Linux::s_Display);
    Linux::s_Display = 0;
    return ResultError(res != 0);
}

Screen WindowSystem::MainScreen(){
    ::Screen *screen = DefaultScreenOfDisplay(Linux::s_Display);

    Size2i size;
    size.width = XWidthOfScreen(screen);
    size.height = XHeightOfScreen(screen);

    Size2f dpi;
    dpi.width = float(size.width) / (float(XWidthMMOfScreen(screen)) / 25.4f);
    dpi.height = float(size.height) / (float(XHeightMMOfScreen(screen)) / 25.4f);

    return Screen(Linux::ScreenImpl(DefaultScreen(Linux::s_Display),screen,size,dpi));
}

}// namespace StraitX::