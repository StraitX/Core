#include "platform/linux/display_x11.hpp"
#include "X11/Xlib.h"
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
    return mHandle ? ErrorCode::Success : ErrorCode::Failure;
}
Error DisplayX11::Close(){
    return XCloseDisplay(mHandle)==0 ? ErrorCode::Success : ErrorCode::Failure;
}

bool DisplayX11::IsOpened(){
    return mHandle;
}

::Display *DisplayX11::Handle(){
    return mHandle;
}

DisplayX11 &DisplayX11::Instance(){
    return display;
}


};// namespace Linux::
};// namespace StraitX::