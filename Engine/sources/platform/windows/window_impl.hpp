#ifndef STRAITX_WINDOWS_WINDOW_HPP
#define STRAITX_WINDOWS_WINDOW_HPP

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

    bool PollEvent(Event &event);

    Size2u Size()const;

    void OnResize(int width, int height);

    sx_inline HWND__ *Handle()const;
private:

    bool FetchInternalEvents(Event &event);
};

sx_inline HWND__ *WindowImpl::Handle() const {
    return m_Handle;
}

}; //namespace Windows::
}; //namespace StraitX::

#endif //STRAITX_WINDOWS_WINDOW_HPP