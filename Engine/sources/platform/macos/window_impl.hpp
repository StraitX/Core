#ifndef STRAITX_MACOS_WINDOW_IMPL_HPP
#define STRAITX_MACOS_WINDOW_IMPL_HPP

#include "platform/result.hpp"
#include "platform/events.hpp"
#include "platform/macos/screen_impl.hpp"

namespace MacOS{

struct WindowImpl{
    void *Handle = nullptr;

    WindowImpl() = default;

    WindowImpl(WindowImpl &&other);

    Result Open(const ScreenImpl &screen, int width, int height);

    Result Close();

    bool IsOpen()const;

    void SetTitle(const char *title);

    bool PollEvent(Event &event);

    Size2u Size()const;
};

}//namespace MacOS::

#endif//STRAITX_MACOS_WINDOW_IMPL_HPP