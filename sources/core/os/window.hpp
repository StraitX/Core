#ifndef STRAITX_WINDOW_HPP
#define STRAITX_WINDOW_HPP

#include "core/env/os.hpp"
#include "core/os/events.hpp"
#include "core/os/screen.hpp"

#if defined(SX_OS_LINUX)
    #include "platform/linux/window_impl.hpp"
    typedef Linux::WindowImpl PlatformWindowImpl;
#elif defined(SX_OS_WINDOWS)
    #include "platform/windows/window_impl.hpp"
    typedef Windows::WindowImpl PlatformWindowImpl;
#elif defined(SX_OS_MACOS)
    #include "platform/macos/window_impl.hpp"
    typedef MacOS::WindowImpl PlatformWindowImpl;
#else
    #error "Your platform does not support window creation"
#endif

class PlatformWindow{
public:
    static void SetTitle(const char *title);

    static Vector2u Size();

	static void SetSize(u32 width, u32 height);

	static void SetSize(Vector2u size);

	static const PlatformScreen &Screen();

	static const PlatformWindowImpl &Impl();
};

SX_INLINE void PlatformWindow::SetTitle(const char *title){
	PlatformWindowImpl::s_MainWindow.SetTitle(title);
}

SX_INLINE Vector2u PlatformWindow::Size(){
	return PlatformWindowImpl::s_MainWindow.Size();
}

SX_INLINE void PlatformWindow::SetSize(u32 width, u32 height){
	PlatformWindowImpl::s_MainWindow.SetSize(width, height);
}

SX_INLINE void PlatformWindow::SetSize(Vector2u size){
	PlatformWindowImpl::s_MainWindow.SetSize(size.x, size.y);
}

SX_INLINE const PlatformScreen &PlatformWindow::Screen(){
	return PlatformWindowImpl::s_MainWindow.Screen();
}

SX_INLINE const PlatformWindowImpl &PlatformWindow::Impl(){
	return PlatformWindowImpl::s_MainWindow;
}

#endif // STRAITX_WINDOW_HPP