#ifndef STRAITX_WINDOW_X11_HPP
#define STRAITX_WINDOW_X11_HPP

#include "platform/events.hpp"
#include "platform/error.hpp"
#include "platform/linux/display_x11.hpp"

namespace StraitX{
namespace Linux{

class WindowX11{
private:
    DisplayX11 &m_Display;
    unsigned long m_Handle = 0;
    void *m_FBConfig = nullptr;
public:

    WindowX11(DisplayX11 &display);

    WindowX11(const WindowX11 &other) = delete;

    WindowX11(WindowX11 &&other);

    Error Open(const ScreenX11 &screen, int width, int height);

    Error Close();

    DisplayX11 &Display();

    unsigned long Handle()const;

    void *FBConfig();

    bool IsOpen() const;

    void SetTitle(const char *title);

    void SetSize(int width, int height);

    bool PollEvent(Event &event);
private:

    void *PickBestFBConfig(const DisplayX11 &display, const ScreenX11 &screen);
};

}; //namespace Linux::
}; //namespace StraitX::

#endif // STRAITX_WINDOW_X11_HPP