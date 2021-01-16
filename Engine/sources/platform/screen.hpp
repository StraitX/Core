#ifndef STRAITX_SCREEN_HPP
#define STRAITX_SCREEN_HPP

#include "platform/platform_detection.hpp"
#include "platform/types.hpp"

#ifdef SX_PLATFORM_LINUX
    #include "platform/linux/screen_impl.hpp"
    typedef StraitX::Linux::ScreenImpl PlatformScreenImpl;
#elif defined(SX_PLATFORM_WINDOWS)
    #include "platform/windows/screen_win32.hpp"
    typedef StraitX::Windows::ScreenWin32 PlatformScreenImpl;
#else 
    #error "Your platform does not support Screen"
#endif

namespace StraitX{

class Screen{
private:
    PlatformScreenImpl m_Impl;
public:
    Screen(const PlatformScreenImpl &impl);

    Screen(const Screen &other) = default;

    ~Screen() = default;

    const Size2i &Size()const;

    const Size2f &DPI()const;

    PlatformScreenImpl &Impl();

    const PlatformScreenImpl &Impl()const;

};

sx_inline Screen::Screen(const PlatformScreenImpl &impl):
    m_Impl(impl)
{}

sx_inline const Size2i &Screen::Size()const{
    return m_Impl.Size();
}

sx_inline const Size2f &Screen::DPI()const{
    return m_Impl.DPI();
}

sx_inline PlatformScreenImpl &Screen::Impl(){
    return m_Impl;
}

sx_inline const PlatformScreenImpl &Screen::Impl()const{
    return m_Impl;
}

}; // namespace StraitX::

#endif // STRAITX_SCREEN_HPP