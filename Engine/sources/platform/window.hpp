#ifndef STRAITX_WINDOW_HPP
#define STRAITX_WINDOW_HPP

#include "platform/platform_detection.hpp"
#include "platform/events.hpp"
#include "platform/screen.hpp"

#if defined(SX_PLATFORM_LINUX)
    #include "platform/linux/window_impl.hpp"
    typedef Linux::WindowImpl PlatformWindowImpl;
#elif defined(SX_PLATFORM_WINDOWS)
    #include "platform/windows/window_impl.hpp"
    typedef Windows::WindowImpl PlatformWindowImpl;
#elif defined(SX_PLATFORM_MACOS)
    #include "platform/macos/window_impl.hpp"
    typedef MacOS::WindowImpl PlatformWindowImpl;
#else
    #error "Your platform does not support window creation"
#endif

class PlatformWindow{
private:
    PlatformWindowImpl m_Impl;
public:
    PlatformWindow(PlatformWindowImpl impl);

    bool IsOpen()const;

    void SetTitle(const char *title);

    Size2u Size()const;

	const PlatformWindowImpl &Impl()const;
};

SX_INLINE PlatformWindow::PlatformWindow(PlatformWindowImpl impl):
	m_Impl(impl)
{}

SX_INLINE bool PlatformWindow::IsOpen()const{
    return m_Impl.IsOpen();
}

SX_INLINE void PlatformWindow::SetTitle(const char *title){
    m_Impl.SetTitle(title);
}

SX_INLINE Size2u PlatformWindow::Size()const{
    return m_Impl.Size();
}

SX_INLINE const PlatformWindowImpl &PlatformWindow::Impl()const{
	return m_Impl;
}
#endif // STRAITX_WINDOW_HPP