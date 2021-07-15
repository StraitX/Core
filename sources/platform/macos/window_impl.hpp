#ifndef STRAITX_MACOS_WINDOW_IMPL_HPP
#define STRAITX_MACOS_WINDOW_IMPL_HPP

#include "platform/result.hpp"
#include "platform/events.hpp"
#include "platform/screen.hpp"
#include "platform/noncopyable.hpp"

namespace MacOS{

struct WindowImpl: NonCopyable{
    using EventHandlerProc = void (*)(const Event &e);

    void *Handle = nullptr;
    void *View = nullptr;
    void *Delegate = nullptr;
    EventHandlerProc EventHandler = nullptr;
    PlatformScreen CurrentScreen;

    static WindowImpl s_MainWindow;

    WindowImpl() = default;

    Result Open(int width, int height);

    void Close();

    void SetTitle(const char *title);

    Size2u Size()const;

    void SetSize(u32 width, u32 height);

    const PlatformScreen &Screen();
};

}//namespace MacOS::

#endif//STRAITX_MACOS_WINDOW_IMPL_HPP