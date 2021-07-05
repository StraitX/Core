#ifndef STRAITX_WINDOWS_WINDOW_HPP
#define STRAITX_WINDOWS_WINDOW_HPP

#include "platform/events.hpp"
#include "platform/result.hpp"
#include "platform/windows/screen_impl.hpp"

struct HWND__;

namespace Windows{

class WindowImpl{
private:
    HWND__ *m_Handle = nullptr;
    bool m_UnhandledResize = false;
public:
    Result Open(const ScreenImpl &screen, int width, int height);

    Result Close();

    bool IsOpen() const;

    void SetTitle(const char *title);

    Size2u Size()const;

    void SetSize(int width, int height);

    SX_INLINE HWND__ *Handle()const;

    static Size2u GetSizeFromHandle(HWND__* handle);
};

SX_INLINE HWND__ *WindowImpl::Handle() const {
    return m_Handle;
}

}//namespace Windows::

#endif //STRAITX_WINDOWS_WINDOW_HPP