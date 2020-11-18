#ifndef STRAITX_WINDOW_X11_HPP
#define STRAITX_WINDOW_X11_HPP

#include "platform/events.hpp"
#include "platform/error.hpp"
#include "platform/linux/fbconfig_x11.hpp"
#include "platform/linux/display_x11.hpp"

namespace StraitX{
namespace Linux{

class WindowX11{
private:
    unsigned long m_Handle = 0;
    DisplayX11 &m_Display;
    FBConfigX11 m_FBConfig;
public:

    WindowX11(DisplayX11 &display);

    WindowX11(const WindowX11 &other) = delete;

    WindowX11(WindowX11 &&other);

    Error Open(const ScreenX11 &screen, int width, int height, const FBConfigX11 &fbconfig);

    Error Close();

    unsigned long Handle()const;

    DisplayX11 &Display();

    const FBConfigX11 &FBConfig()const;

    bool IsOpen() const;

    void SetTitle(const char *title);

    void SetSize(int width, int height);

    bool PollEvent(Event &event);
};

}; //namespace Linux::
}; //namespace StraitX::

#endif // STRAITX_WINDOW_X11_HPP