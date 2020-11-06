#ifndef STRAITX_WINDOW_HPP
#define STRAITX_WINDOW_HPP

#include "platform/platform_detection.hpp"
#include "platform/events.hpp"

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
    WindowImpl mImpl;
public:
    Window(int width, int height, const char *title);
    ~Window();

    void SetTitle(const char *title);

    void SetSize(int width, int height);

    bool PollEvent(Event &event);
};

}; // namespace StraitX::

#endif // STRAITX_WINDOW_HPP