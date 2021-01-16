#ifndef STRAITX_LINUX_WINDOW_IMPL_HPP
#define STRAITX_LINUX_WINDOW_IMPL_HPP

#include "platform/events.hpp"
#include "platform/result.hpp"
#include "platform/linux/screen_impl.hpp"

namespace StraitX{
namespace Linux{

struct WindowImpl{
    unsigned long Handle = 0;
    void *FBConfig = nullptr;

    WindowImpl() = default;

    WindowImpl(const WindowImpl &other) = default;

    WindowImpl(WindowImpl &&other);

    Result Open(const ScreenImpl &screen, int width, int height);

    Result Close();

    bool IsOpen() const;

    void SetTitle(const char *title);

    void SetSize(int width, int height);

    bool PollEvent(Event &event);

    static void *PickBestFBConfig(int screen_index);
};

} //namespace Linux::
} //namespace StraitX::

#endif // STRAITX_LINUX_WINDOW_IMPL_HPP