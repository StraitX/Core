#ifndef STRAITX_MACOS_SCREEN_IMPL_HPP
#define STRAITX_MACOS_SCREEN_IMPL_HPP

#include "platform/result.hpp"
#include "platform/events.hpp"

namespace MacOS{

class ScreenImpl{
private:
    Size2i m_Size;
    Size2f m_DPI;
public:
    constexpr ScreenImpl(Size2i size, Size2f dpi);

    constexpr const Size2i &Size()const;

    constexpr const Size2f &DPI()const;
};

constexpr ScreenImpl::ScreenImpl(Size2i size, Size2f dpi):
    m_Size(size),
    m_DPI(dpi)
{}

constexpr const Size2i &ScreenImpl::Size()const{
    return m_Size;
}

constexpr const Size2f &ScreenImpl::DPI()const{
    return m_DPI;
}

}//namespace MacOS::

#endif//STRAITX_MACOS_SCREEN_IMPL_HPP