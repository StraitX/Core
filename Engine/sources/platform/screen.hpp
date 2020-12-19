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
    sx_inline Screen(const ScreenImpl &impl):
        m_Impl(impl)
    {}

    Screen(const Screen &other) = default;

    ~Screen() = default;

    sx_inline const Size2i &Size()const{
        return m_Impl.Size();
    }

    sx_inline const Size2f &DPI()const{
        return m_Impl.DPI();
    }

    sx_inline ScreenImpl &Impl(){
        return m_Impl;
    }

    sx_inline const ScreenImpl &Impl()const{
        return m_Impl;
    }

};

}; // namespace StraitX::

#endif // STRAITX_SCREEN_HPP