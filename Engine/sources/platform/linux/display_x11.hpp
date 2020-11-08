#ifndef STRAITX_LINUX_DISPLAY_HPP
#define STRAITX_LINUX_DISPLAY_HPP

#include "platform/error.hpp"
#include <X11/Xlib.h>

namespace StraitX{
namespace Linux{

class DisplayX11{
private:
    ::Display *mHandle;
public:
    DisplayX11();

    Error Open();
    Error Close();

    bool IsOpened();

    ::Display *Handle();

    static DisplayX11 &Instance();
};

};// namespace Linux::
};// namespace StraitX::

#endif