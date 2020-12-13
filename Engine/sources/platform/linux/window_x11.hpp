#ifndef STRAITX_WINDOW_X11_HPP
#define STRAITX_WINDOW_X11_HPP

#include "platform/events.hpp"
#include "platform/error.hpp"
#include "platform/linux/display_x11.hpp"

namespace StraitX{
namespace Linux{

struct WindowX11{
    DisplayX11 &Display;
    unsigned long Handle = 0;
    void *FBConfig = nullptr;

    WindowX11(DisplayX11 &display);

    WindowX11(const WindowX11 &other) = delete;

    WindowX11(WindowX11 &&other);

    Error Open(const ScreenX11 &screen, int width, int height);

    Error Close();

    bool IsOpen() const;

    void SetTitle(const char *title);

    void SetSize(int width, int height);

    bool PollEvent(Event &event);

    static void *PickBestFBConfig(const DisplayX11 &display, const ScreenX11 &screen);
};

}; //namespace Linux::
}; //namespace StraitX::

#endif // STRAITX_WINDOW_X11_HPP