#ifndef STRAITX_WINDOWS_WINDOW_HPP
#define STRAITX_WINDOWS_WINDOW_HPP

#include "platform/events.hpp"
#include "platform/result.hpp"
#include "platform/screen.hpp"
#include "platform/noncopyable.hpp"

struct HWND__;

namespace Windows{

class WindowImpl: public NonCopyable{
private:
    HWND__ *m_Handle = nullptr;
    bool m_UnhandledResize = false;
    PlatformScreen m_Screen;
public:
    static WindowImpl s_MainWindow;

    Result Open(int width, int height);

    Result Close();

    void SetTitle(const char *title);

    Size2u Size()const;

    void SetSize(int width, int height);

    const PlatformScreen& Screen();

    SX_INLINE HWND__ *Handle()const;

    static Size2u GetSizeFromHandle(HWND__* handle);

    static Result RegisterWindowClass();
};

SX_INLINE HWND__ *WindowImpl::Handle() const {
    return m_Handle;
}

}//namespace Windows::

#endif //STRAITX_WINDOWS_WINDOW_HPP