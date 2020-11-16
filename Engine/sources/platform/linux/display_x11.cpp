#include "platform/linux/display_x11.hpp"
#include "X11/Xlib.h"
#include "platform/linux/screen_x11.hpp"
#include "platform/types.hpp"
//undef X11 macro to resolve conflicts
#undef Success
#include "platform/error.hpp"


namespace StraitX{
namespace Linux{

static DisplayX11 display;


DisplayX11::DisplayX11():
    mHandle(nullptr)
{}

Error DisplayX11::Open(){
    mHandle = XOpenDisplay(nullptr);
    return mHandle != nullptr ? Error::Success : Error::Failure;
}
Error DisplayX11::Close(){
    return XCloseDisplay(mHandle)==0 ? Error::Success : Error::Failure;
}

bool DisplayX11::IsOpened(){
    return mHandle;
}

ScreenX11 DisplayX11::MainScreen(){
    Screen *screen = DefaultScreenOfDisplay(mHandle);

    Size2i size;
    size.width = XWidthOfScreen(screen);
    size.height = XHeightOfScreen(screen);

    Size2f dpi;
    dpi.width = float(size.width) / (float(XWidthMMOfScreen(screen)) / 25.4f);
    dpi.height = float(size.height) / (float(XHeightMMOfScreen(screen)) / 25.4f);

    return ScreenX11(DefaultScreen(mHandle),screen,size,dpi);
}

::Display *DisplayX11::Handle(){
    return mHandle;
}

DisplayX11 &DisplayX11::Instance(){
    return display;
}


};// namespace Linux::
};// namespace StraitX::