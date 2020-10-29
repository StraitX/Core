#include "platform/linux/display.hpp"
#include "X11/Xlib.h"
//undef X11 macro to resolve conflicts
#undef Success
#include "platform/error.hpp"


namespace StraitX{
namespace Linux{

static Display display;


Display::Display():
    mHandle(nullptr)
{}

int Display::Open(){
    mHandle = XOpenDisplay(nullptr);
    return mHandle ? ErrorCode::Success : ErrorCode::Failure;
}
int Display::Close(){
    return XCloseDisplay(mHandle);
}

::Display *Display::Handle(){
    return mHandle;
}

Display &Display::Instance(){
    return display;
}


};// namespace Linux::
};// namespace StraitX::