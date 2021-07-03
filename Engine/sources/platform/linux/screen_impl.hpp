#ifndef STRAITX_LINUX_SCREEN_IMPL_HPP
#define STRAITX_LINUX_SCREEN_IMPL_HPP

#include "platform/types.hpp"


namespace Linux{

struct ScreenImpl{
    void *Handle = nullptr;
    unsigned int Index = -1;
    Size2i Size = {};
    Size2f DPI = {};

	ScreenImpl() = default;

	ScreenImpl(void *display_handle);
};

} // namespace Linux::


#endif // STRAITX_LINUX_SCREEN_IMPL_HPP