#ifndef STRAITX_WINDOW_X11_HPP
#define STRAITX_WINDOW_X11_HPP

#include "platform/events.hpp"
#include "platform/error.hpp"
#include "platform/linux/fbconfig_x11.hpp"
#include "platform/display.hpp"

namespace StraitX{
namespace Linux{

class WindowX11{
private:
    unsigned long mHandle;
    int mScreenIndex;
public:
    WindowX11();

    WindowX11(const WindowX11 &other) = delete;

    WindowX11(WindowX11 &&other);

    Error Open(int width, int height, const FBConfigX11 &fbconfig);

    Error Close();

    bool IsOpen() const;

    void SetTitle(const char *title);

    void SetSize(int width, int height);

    bool PollEvent(Event &event);
};

}; //namespace Linux::
}; //namespace StraitX::

#endif // STRAITX_WINDOW_X11_HPP