#ifndef STRAITX_LINUX_DISPLAY_HPP
#define STRAITX_LINUX_DISPLAY_HPP

#include "platform/error.hpp"
#include "platform/linux/screen_x11.hpp"

struct _XDisplay;

namespace StraitX{
namespace Linux{

class DisplayX11{
private:
    ::_XDisplay *mHandle = nullptr;
public:
    DisplayX11() = default;

    Error Open();

    Error Close();

    bool IsOpened();

    ScreenX11 MainScreen();

    ::_XDisplay *Handle();
};

};// namespace Linux::
};// namespace StraitX::

#endif