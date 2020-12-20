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

    Screen(const Screen &other) = default;

    ~Screen() = default;

    const Size2i &Size()const;

    const Size2f &DPI()const;

    ScreenImpl &Impl();

    const ScreenImpl &Impl()const;

};

sx_inline Screen::Screen(const ScreenImpl &impl):
    m_Impl(impl)
{}

sx_inline const Size2i &Screen::Size()const{
    return m_Impl.Size();
}

sx_inline const Size2f &Screen::DPI()const{
    return m_Impl.DPI();
}

sx_inline ScreenImpl &Screen::Impl(){
    return m_Impl;
}

sx_inline const ScreenImpl &Screen::Impl()const{
    return m_Impl;
}

}; // namespace StraitX::

#endif // STRAITX_SCREEN_HPP