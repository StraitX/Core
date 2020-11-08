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
    int mWidth;
    int mHeight;
    bool mWindowShouldClose : 1;
    bool mUnhandledResize : 1;
    friend class Platform;
public:
    WindowWin32(int width, int height);
    ~WindowWin32();

    void SetTitle(const char *title);

    void SetSize(int width, int height);

    bool PollEvent(Event &event);

    void OnResize(int width, int height);

    void OnClose();
private:
    bool FetchInternalEvents(Event &event);

    static void RegClass();
};

}; //namespace Windows::
}; //namespace StraitX::

#endif // STRAITX_WINDOW_WIN32_HPP