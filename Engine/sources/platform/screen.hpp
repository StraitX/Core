#ifndef STRAITX_SCREEN_HPP
#define STRAITX_SCREEN_HPP

#include "platform/platform_detection.hpp"
#include "platform/types.hpp"

#ifdef SX_PLATFORM_LINUX
    #include "platform/linux/screen_x11.hpp"
    typedef StraitX::Linux::ScreenX11 ScreenImpl;
#elif defined(SX_PLATFORM_WINDOWS)
    #include "platform/windows/screen_win32.hpp"
    typedef StraitX::Windows::ScreenWin32 ScreenImpl;
#else 
    #error "Your platform does not support Screen"
#endif

namespace StraitX{

class Screen{
private:
    ScreenImpl m_Impl;
public:
    Screen(const ScreenImpl &impl);

    ~Screen() = default;

    const Size2i &Size()const;

    const Size2f &DPI()const;

    ScreenImpl &Impl();

    const ScreenImpl &Impl()const;

};

}; // namespace StraitX::

#endif // STRAITX_SCREEN_HPP