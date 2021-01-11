#ifndef STRAITX_WINDOW_WIN32_HPP
#define STRAITX_WINDOW_WIN32_HPP

#include "platform/events.hpp"
#include "platform/result.hpp"
#include "platform/windows/screen_win32.hpp"
#include "platform/windows/display_win32.hpp"

namespace StraitX {
class Platform;
}; // namespace StraitX::

// struct HWND__;

namespace StraitX{
namespace Windows{

class WindowWin32{
private:
    DisplayWin32& m_Display;
    void *m_Handle;
    int m_Width;
    int m_Height;
    bool m_UnhandledResize;
    friend class Platform;
public:
    WindowWin32(DisplayWin32& display);

    WindowWin32(const WindowWin32& other) = delete;

    WindowWin32(WindowWin32&& other);

    Result Open(const ScreenWin32 &screen, int width, int height);

    Result Close();

    bool IsOpen()const;

    void SetTitle(const char *title);

    void SetSize(int width, int height);

    bool PollEvent(Event &event);

    void OnResize(int width, int height);
private:
    bool FetchInternalEvents(Event &event);
};

}; //namespace Windows::
}; //namespace StraitX::

#endif // STRAITX_WINDOW_WIN32_HPP