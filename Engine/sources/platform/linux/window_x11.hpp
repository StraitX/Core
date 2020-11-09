#ifndef STRAITX_WINDOW_X11_HPP
#define STRAITX_WINDOW_X11_HPP

#include "platform/events.hpp"

namespace StraitX{
namespace Linux{

class WindowX11{
private:
    unsigned long mHandle;
    int mScreenIndex;
public:
    WindowX11(int width, int height);

    WindowX11(const WindowX11 &other) = delete;

    WindowX11(WindowX11 &&other);

    ~WindowX11();

    bool IsOpen();

    void SetTitle(const char *title);

    void SetSize(int width, int height);

    bool PollEvent(Event &event);
};

}; //namespace Linux::
}; //namespace StraitX::

#endif // STRAITX_WINDOW_X11_HPP