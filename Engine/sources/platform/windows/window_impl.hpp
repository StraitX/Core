#ifndef STRAITX_WINDOW_WIN32_HPP
#define STRAITX_WINDOW_WIN32_HPP

#include "platform/events.hpp"
#include "platform/result.hpp"
#include "platform/windows/screen_impl.hpp"

struct HWND__;

namespace StraitX{
namespace Windows{

class WindowImpl{
private:
    HWND__ *m_Handle = nullptr;
    int m_Width = 0;
    int m_Height = 0;
    bool m_UnhandledResize = false;
public:
    WindowImpl() = default;

    WindowImpl(const WindowImpl& other) = default;

    WindowImpl(WindowImpl&& other);

    Result Open(const ScreenImpl &screen, int width, int height);

    Result Close();

    bool IsOpen() const;

    void SetTitle(const char *title);

    void SetSize(int width, int height);

    bool PollEvent(Event &event);

    Size2u Size()const;

    void OnResize(int width, int height);
private:

    bool FetchInternalEvents(Event &event);
};

}; //namespace Windows::
}; //namespace StraitX::

#endif // STRAITX_WINDOW_WIN32_HPP