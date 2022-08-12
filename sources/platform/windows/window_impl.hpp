#ifndef STRAITX_WINDOWS_WINDOW_HPP
#define STRAITX_WINDOWS_WINDOW_HPP

#include "core/os/events.hpp"
#include "core/result.hpp"
#include "core/os/screen.hpp"
#include "core/function.hpp"
#include "core/noncopyable.hpp"
#include "core/string_view.hpp"

struct HWND__;

namespace Windows{

class WindowImpl: public NonCopyable{
private:
    HWND__ *m_Handle = nullptr;
    bool m_UnhandledResize = false;
    Function<void(const Event& e)> m_EventsHandler;
    mutable Screen m_CurrentScreen;
public:

    Result Open(int width, int height, StringView title);

    Result Close();

    bool IsOpen()const;

    void SetEventsHandler(Function<void(const Event& e)> handler);

    Function<void(const Event& e)> EventsHandler()const {
        return m_EventsHandler;
    }

    void DispatchEvents();

    void SetTitle(StringView title);

    Vector2u Size()const;

    void SetSize(int width, int height);

    const Screen& CurrentScreen()const;

    SX_INLINE HWND__ *Handle()const;
};

SX_INLINE HWND__ *WindowImpl::Handle() const {
    return m_Handle;
}

}//namespace Windows::

#endif //STRAITX_WINDOWS_WINDOW_HPP