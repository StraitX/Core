#ifndef STRAITX_WINDOW_HPP
#define STRAITX_WINDOW_HPP

#include "platform/platform_detection.hpp"
#include "platform/events.hpp"
#include "platform/screen.hpp"
#include "platform/noncopyable.hpp"

#ifdef SX_PLATFORM_LINUX
    #include "platform/linux/window_impl.hpp"
    typedef StraitX::Linux::WindowImpl PlatformWindowImpl;
#elif defined(SX_PLATFORM_WINDOWS)
    #include "platform/windows/window_win32.hpp"
    typedef StraitX::Windows::WindowWin32 WindowImpl;
#else
    #error "Your platform does not support window creation"
#endif

namespace StraitX{

class Window: public NonCopyable{
private:
    PlatformWindowImpl m_Impl;
public:
    Window() = default;

    Window(Window &&other);

    Result Open(const Screen &screen, int width, int height);

    Result Close();

    PlatformWindowImpl &Impl();

    const PlatformWindowImpl &Impl()const;

    bool IsOpen()const;

    void SetTitle(const char *title);

    bool PollEvent(Event &event);
};

sx_inline Window::Window(Window &&other):
    m_Impl((PlatformWindowImpl&&)other.Impl())
{}

sx_inline Result Window::Open(const Screen &screen, int width, int height){
    return m_Impl.Open(screen.Impl(), width, height);
}

sx_inline Result Window::Close(){
    return m_Impl.Close();
}

sx_inline PlatformWindowImpl &Window::Impl(){
    return m_Impl;
}

sx_inline const PlatformWindowImpl &Window::Impl()const{
    return m_Impl;
}

sx_inline bool Window::IsOpen()const{
    return m_Impl.IsOpen();
}

sx_inline void Window::SetTitle(const char *title){
    m_Impl.SetTitle(title);
}

sx_inline bool Window::PollEvent(Event &event){
    return m_Impl.PollEvent(event);
}

}; // namespace StraitX::

#endif // STRAITX_WINDOW_HPP