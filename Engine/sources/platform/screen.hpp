#ifndef STRAITX_SCREEN_HPP
#define STRAITX_SCREEN_HPP

#include "platform/platform_detection.hpp"
#include "platform/types.hpp"

#ifdef SX_PLATFORM_LINUX
    #include "platform/linux/screen_impl.hpp"
    typedef Linux::ScreenImpl PlatformScreenImpl;
#elif defined(SX_PLATFORM_WINDOWS)
    #include "platform/windows/screen_impl.hpp"
    typedef Windows::ScreenImpl PlatformScreenImpl;
#elif defined(SX_PLATFORM_MACOS)
    #include "platform/macos/screen_impl.hpp"
    typedef MacOS::ScreenImpl PlatformScreenImpl;
#else
    #error "Your platform does not support Screen"
#endif

class PlatformScreen{
private:
    PlatformScreenImpl m_Impl;
public:
    PlatformScreen(const PlatformScreenImpl &impl);

    PlatformScreen(const PlatformScreen &other) = default;

    ~PlatformScreen() = default;

    const Size2i &Size()const;

    const Size2f &DPI()const;

    PlatformScreenImpl &Impl();

    const PlatformScreenImpl &Impl()const;

};

SX_INLINE PlatformScreen::PlatformScreen(const PlatformScreenImpl &impl):
    m_Impl(impl)
{}

SX_INLINE const Size2i &PlatformScreen::Size()const{
    return m_Impl.Size;
}

SX_INLINE const Size2f &PlatformScreen::DPI()const{
    return m_Impl.DPI;
}

SX_INLINE PlatformScreenImpl &PlatformScreen::Impl(){
    return m_Impl;
}

SX_INLINE const PlatformScreenImpl &PlatformScreen::Impl()const{
    return m_Impl;
}

#endif // STRAITX_SCREEN_HPP