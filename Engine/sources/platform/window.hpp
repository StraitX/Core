#ifndef STRAITX_WINDOW_HPP
#define STRAITX_WINDOW_HPP

#include "platform/platform_detection.hpp"
#include "platform/events.hpp"
#include "platform/fbconfig.hpp"
#include "platform/screen.hpp"
#include "platform/display.hpp"

#ifdef SX_PLATFORM_LINUX
    #include "platform/linux/window_x11.hpp"
    typedef StraitX::Linux::WindowX11 WindowImpl;
#elif defined(SX_PLATFORM_WINDOWS)
    #include "platform/windows/window_win32.hpp"
    typedef StraitX::Windows::WindowWin32 WindowImpl;
#else
    #error "Your platform does not support window creation"
#endif

namespace StraitX{

class Window{
private:
    WindowImpl m_Impl;
public:

    Window(Display &display);

    Window(const Window &other) = delete;

    Window(Window &&other);

    ~Window();

    Error Open(const Screen &screen, int width, int height, const FBConfig &config);

    Error Close();

    WindowImpl &Impl();

    const WindowImpl &Impl()const;

    bool IsOpen()const;

    void SetTitle(const char *title);

    void SetSize(int width, int height);

    bool PollEvent(Event &event);
};

}; // namespace StraitX::

#endif // STRAITX_WINDOW_HPP