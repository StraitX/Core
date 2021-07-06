#ifndef STRAITX_MACOS_SCREEN_IMPL_HPP
#define STRAITX_MACOS_SCREEN_IMPL_HPP

#include "platform/result.hpp"
#include "platform/compiler.hpp"
#include "platform/events.hpp"

namespace MacOS{

struct ScreenImpl{
    Size2i Size;
    Size2f DPI;

    ScreenImpl(Size2i size, Size2f dpi);
};

SX_INLINE ScreenImpl::ScreenImpl(Size2i size, Size2f dpi):
    Size(size),
    DPI(dpi)
{}

}//namespace MacOS::

#endif//STRAITX_MACOS_SCREEN_IMPL_HPP