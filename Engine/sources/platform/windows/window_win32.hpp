#ifndef STRAITX_WINDOW_WIN32_HPP
#define STRAITX_WINDOW_WIN32_HPP

#include "platform/events.hpp"

namespace StraitX {
class Platform;
}; // namespace StraitX::

namespace StraitX{
namespace Windows{

class WindowWin32{
private:
    void *mHandle;
    friend class Platform;
public:
    WindowWin32(int width, int height);
    ~WindowWin32();

    void SetTitle(const char *title);

    void SetSize(int width, int height);

    bool PollEvent(Event &event);

private:
    static void RegClass();
};

}; //namespace Windows::
}; //namespace StraitX::

#endif // STRAITX_WINDOW_WIN32_HPP