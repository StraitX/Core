#include "platform/linux/display_x11.hpp"
#include "X11/Xlib.h"
#include "platform/linux/screen_x11.hpp"
#include "platform/types.hpp"
//undef X11 macro to resolve conflicts
#undef Success
#include "platform/error.hpp"


namespace StraitX{
namespace Linux{

Error DisplayX11::Open(){
    m_Handle = XOpenDisplay(nullptr);
    return m_Handle != nullptr ? Error::Success : Error::Failure;
}
Error DisplayX11::Close(){
    return XCloseDisplay(m_Handle)==0 ? Error::Success : Error::Failure;
}

bool DisplayX11::IsOpened(){
    return m_Handle;
}

ScreenX11 DisplayX11::MainScreen(){
    Screen *screen = DefaultScreenOfDisplay(m_Handle);

    Size2i size;
    size.width = XWidthOfScreen(screen);
    size.height = XHeightOfScreen(screen);

    Size2f dpi;
    dpi.width = float(size.width) / (float(XWidthMMOfScreen(screen)) / 25.4f);
    dpi.height = float(size.height) / (float(XHeightMMOfScreen(screen)) / 25.4f);

    return ScreenX11(DefaultScreen(m_Handle),screen,size,dpi);
}

::Display *DisplayX11::Handle(){
    return m_Handle;
}


};// namespace Linux::
};// namespace StraitX::